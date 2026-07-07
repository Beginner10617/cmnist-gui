#include "train_utils.h"
#include "neuron.h"
#include "fileSystem.h"

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
}

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

int main(){
	FILE *fptrimg, *fptrlabel;
	printf("loading dataset...\n");
	fptrimg = fopen("dataset/t10k-images-idx3-ubyte", "rb");
	fptrlabel = fopen("dataset/t10k-labels-idx1-ubyte","rb");
	if (fptrimg == NULL || fptrlabel == NULL) {
    		printf("Error opening file!\n");
		return 1;
	}

	MLP* mlp = loadMLP("model.txt");
	if(mlp==NULL){
		printf("Error loading model\n");
		exit(1);
	}

	uint32_t imgheader[4], labelheader[2];
	readImgHeader(imgheader, fptrimg);
	readLabelHeader(labelheader, fptrlabel);
	int image_size = imgheader[2] * imgheader[3];
	
	uint8_t label = 0;
	uint8_t image[image_size];
	
	int z, total=0, corr=0;
	Value** imageVal; Value** ypred;

	while(readNextImage(image, image_size, fptrimg)){
		imageVal = imgDataToValueArray(image, image_size);

		if(fread(&label, 1, 1, fptrlabel) != 1){
			printf("Error reading label\n");
			break;
		}

		ypred = evaluateMLP(mlp, imageVal);
		float min = -1.0f;
		for(int abc=0; abc<10; abc++){
			if(ypred[abc]->data > min){
				min = ypred[abc]->data;
				z = abc;
			}
		}
		if(z==label)corr++;
		total++;
		if(total >= imgheader[1]) break;
	}
	printf("%d images read succedfully\n%d correct outputs, %.2f%% accuracy\n", 
			total, corr, corr * 100.0f / total);
    	fclose(fptrimg);
	fclose(fptrlabel);
	return 0;
}
