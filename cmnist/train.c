#include "train_utils.h"
#include "fileSystem.h"
#include "time.h"
#include "stdio.h"
#include "neuron.h"

#define BAR_WIDTH 50 // For progress bar

Value** labelToValueArray(int x){
	if(x<0 || x>9) return NULL;
	Value** out = malloc(sizeof(Value*) * 10);
	for(int i=0; i<10; i++){
		if(i==x)
			out[i] = createNewValue(1.0f,"x");
		else
			out[i] = createNewValue(0.0f, "x");
		sprintf(out[i]->name, "%d", i);
		out[i]->_isparameter = 1;
	}
	return out;
};

Value** imgDataToValueArray(uint8_t *img, int img_sz){
	// convert image data [0,255] into an array
	// of same size with values [0.0f, 1.0f]
	// Use linear scaling
	Value** out = malloc(sizeof(Value*) * img_sz);
	for(int i=0; i<img_sz; i++){
		out[i] = createNewValue(img[i]/127.5f - 1,"x");
		sprintf(out[i]->name, "%d",i);
		out[i]->_isparameter = 1;
	}
	return out;
}

void train(int iterations, float stepSize){
	
	FILE *fptrimg, *fptrlabel;
	
	fptrimg = fopen("dataset/train-images-idx3-ubyte", "rb");
	fptrlabel = fopen("dataset/train-labels-idx1-ubyte","rb");
	
	if (fptrimg == NULL || fptrlabel == NULL) {
    		printf("Error opening file!\n");
		return ;
	}

	// loading dataset header
	uint32_t imgheader[4], labelheader[2];
	readImgHeader(imgheader, fptrimg);
	readLabelHeader(labelheader, fptrlabel);
	
	// loading data from the dataset
	int image_size = imgheader[2] * imgheader[3];
	uint8_t label = 0;
	Value*** ground_truth = malloc(sizeof(Value**) * labelheader[1]);
	Value*** img_inputs = malloc(sizeof(Value**) * imgheader[1]);
	uint8_t image[image_size];
		
	for(int i=0; i<imgheader[1]; i++){
		if(fread(&label, 1, 1, fptrlabel) != 1){
			printf("Error reading label\n");
			break;
		}
		
		if(!readNextImage(image, image_size, fptrimg)) break;
		// load data to Value ground_truth[i]->{_, _, _, _, _, _, _, _, _, _}, 
		// img_inputs[i] : convert [0,256) -> [-1,1], 
		ground_truth[i] = labelToValueArray(label);
		img_inputs[i] = imgDataToValueArray(image, image_size);
	}
    
	fclose(fptrimg);
	fclose(fptrlabel);
	printf("Image data loaded!\n");
	printf("Opening MLP...\n");
	MLP* mlp = loadMLP("model.txt");
	int tanh[] = {1,1,0};
	int outputs[] = {32,16,10};
	if(mlp==NULL){
		printf("model not found, creating new...\n");
		mlp = createMLP(image_size, 3, outputs, tanh, "ADAM");
	}
	
	printf("Allocating memory for training data...\n");
	
	int batch_size = 10;
	Value*** ypred = malloc(sizeof(Value**) * batch_size);
	Value***  dely = malloc(sizeof(Value**) * batch_size);
	Value*** sqdely= malloc(sizeof(Value**) * batch_size);
	Value** devn = malloc(sizeof(Value*) * batch_size);
	Value* losssum; float currLoss;
	
	printf("Starting training loop...\n");
	int asd, num_of_batches = imgheader[1] / batch_size;

	// training
for(int iter = 0; iter<iterations; iter++){
	for(int j=0; j < num_of_batches; j++){
		int off = j * batch_size;
		for(int ipt=0; ipt < batch_size && ipt + off < imgheader[1]; ipt++){
			ypred[ipt] = evaluateMLP(mlp, img_inputs[ipt + off]);
			dely[ipt] = subValueArr(ground_truth[ipt + off], ypred[ipt], 10);
			sqdely[ipt] = sqValueArr(dely[ipt], 10);
			devn[ipt] = sum(sqdely[ipt], 10);
		}
			
		losssum = sum(devn, batch_size);
		currLoss = losssum->data;
		backPropagate(losssum);
		freeComputationTree(losssum);
		gradientDescentMLP(mlp, stepSize);
		
		// progress bar
		printf("\r[");	
		int pos = ((j+1) * BAR_WIDTH) / (num_of_batches);

		for (int i = 0; i < BAR_WIDTH; i++) {
			if (i < pos)
				printf("=");
			else if (i == pos)
				printf(">");
			else
				printf(" ");
		}

		printf("] iteration %d/%d batch %d/%d loss = %f", iter+1, 
			iterations, j+1, num_of_batches, currLoss);
		fflush(stdout);
	}
	saveMLP(mlp, "model.txt");
	printf(" saved model!\n");
}
}

int main(){
	srand((unsigned int)time(NULL));
	train(10, 0.005f);
	return 0;
}
