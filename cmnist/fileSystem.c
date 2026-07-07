#include "fileSystem.h"
#include "stdlib.h"

void saveMLP(MLP* mlp, char* Fname){
	FILE *file;	
	file = fopen(Fname, "w");
	if (file == NULL){
		printf("Error opening file %s\n", Fname);
		exit(1);
	}
	
	fprintf(file, "%s\n", mlp->name);
	
	fprintf(file, "%d\n%d\n%d\n",mlp->num_of_outputs,
		mlp->num_of_inputs, mlp->num_of_layers);

	for(int i=0; i<mlp->num_of_layers; i++){
		fprintf(file, "%s\n", mlp->layers[i]->name);

		fprintf(file, "%d\n%d\n%d\n", 
				mlp->layers[i]->num_of_neurons,
				mlp->layers[i]->dim_of_neurons,
				mlp->actfunc[i]);

		for(int j=0; j<mlp->layers[i]->num_of_neurons; j++){
	
			fprintf(file, "%s\n", mlp->layers[i]->neurons[j]->name);
	
			fprintf(file, "%d\n", mlp->layers[i]->neurons[j]->dimension);
	
			fprintf(file, "%f\n", mlp->layers[i]->neurons[j]->bias->data);
	
			for(int k=0; k<mlp->layers[i]->neurons[j]->dimension; k++){
				fprintf(file, "%f\n", mlp->layers[i]->neurons[j]->weights[k]->data);
			}
		}
	}
	fclose(file);
}

MLP* loadMLP(char* Fname){
	char line[20];
	FILE *file;
	file = fopen(Fname, "r");
	if(file == NULL){
		printf("Error opening file %s\n", Fname);
		return NULL;	
	}
	MLP* mlp = malloc(sizeof(MLP));

	fgets(mlp->name, sizeof(mlp->name), file);
	mlp->name[strcspn(mlp->name, "\n")]='\0';
		
	fgets(line, sizeof(line), file);
	mlp->num_of_outputs = atoi(line);
	
	fgets(line, sizeof(line), file);
	mlp->num_of_inputs = atoi(line);
	
	fgets(line, sizeof(line), file);
	mlp->num_of_layers = atoi(line);

	mlp->layers = malloc(sizeof(Layer*) * mlp->num_of_layers);
	mlp->actfunc= malloc(sizeof(int) * mlp->num_of_layers);
	for(int i=0; i<mlp->num_of_layers; i++){
		mlp->layers[i] = malloc(sizeof(Layer));
		fgets(mlp->layers[i]->name, sizeof(mlp->layers[i]->name), file);
		mlp->layers[i]->name[strcspn(mlp->layers[i]->name, "\n")]='\0';
		
		fgets(line, sizeof(line), file);
		mlp->layers[i]->num_of_neurons = atoi(line);
		fgets(line, sizeof(line), file);
		mlp->layers[i]->dim_of_neurons = atoi(line);
		fgets(line, sizeof(line), file);
		mlp->actfunc[i] = atoi(line);

		mlp->layers[i]->neurons = malloc(
		sizeof(Neuron*) * mlp->layers[i]->num_of_neurons);

		for(int j=0; j<mlp->layers[i]->num_of_neurons; j++){
			mlp->layers[i]->neurons[j] = malloc(sizeof(Neuron));
			
			fgets(mlp->layers[i]->neurons[j]->name,
			sizeof(mlp->layers[i]->neurons[j]->name),
			file);
			mlp->layers[i]->neurons[j]->name[
			strcspn(mlp->layers[i]->neurons[j]->name, "\n")
			] = '\0';
			
			fgets(line, sizeof(line), file);
			mlp->layers[i]->neurons[j]->dimension = atoi(line);

			fgets(line, sizeof(line), file);
			mlp->layers[i]->neurons[j]->bias = createNewValue(atof(line),"b");
			mlp->layers[i]->neurons[j]->bias->_isparameter = 1;

			mlp->layers[i]->neurons[j]->weights = malloc(
				sizeof(Value*) * mlp->layers[i]->neurons[j]->dimension);
			
			for(int k=0; k<mlp->layers[i]->neurons[j]->dimension; k++){
				fgets(line, sizeof(line), file);
				mlp->layers[i]->neurons[j]->weights[k] = createNewValue(
					atof(line), "w");
				snprintf(mlp->layers[i]->neurons[j]->weights[k]->name,
					sizeof(mlp->layers[i]->neurons[j]->weights[k]->name),
					"w%d", i);
				mlp->layers[i]->neurons[j]->weights[k]->_isparameter = 1;
			}
		}
	}
	fclose(file);
	return mlp;
}
