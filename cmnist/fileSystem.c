#include "fileSystem.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void saveMLP(MLP *mlp, const char *Fname) {
  FILE *file;
  file = fopen(Fname, "w");
  if (file == NULL) {
    printf("Error opening file %s\n", Fname);
    exit(1);
  }

  fprintf(file, "%s\n", mlp->name);

  fprintf(file, "%d\n%d\n%d\n", mlp->num_of_outputs, mlp->num_of_inputs,
          mlp->num_of_layers);

  for (int i = 0; i < mlp->num_of_layers; i++) {
    fprintf(file, "%s\n", mlp->layers[i]->name);

    fprintf(file, "%d\n%d\n%d\n", mlp->layers[i]->num_of_neurons,
            mlp->layers[i]->dim_of_neurons, mlp->actfunc[i]);

    for (int j = 0; j < mlp->layers[i]->num_of_neurons; j++) {

      fprintf(file, "%s\n", mlp->layers[i]->neurons[j]->name);

      fprintf(file, "%d\n", mlp->layers[i]->neurons[j]->dimension);

      fprintf(file, "%f\n", mlp->layers[i]->neurons[j]->bias->data);

      for (int k = 0; k < mlp->layers[i]->neurons[j]->dimension; k++) {
        fprintf(file, "%f\n", mlp->layers[i]->neurons[j]->weights[k]->data);
      }
    }
  }
  fclose(file);
}

MLP *loadMLP(const char *Fname) {
  char line[20];
  FILE *file;
  file = fopen(Fname, "r");
  if (file == NULL) {
    printf("Error opening file %s\n", Fname);
    return NULL;
  }
  int ecode = validate(Fname);
  if (ecode) {
    printf("Error (code %d) parsing file %s\n", ecode, Fname);
    return NULL;
  }
  MLP *mlp = malloc(sizeof(MLP));

  fgets(mlp->name, sizeof(mlp->name), file);
  mlp->name[strcspn(mlp->name, "\n")] = '\0';

  fgets(line, sizeof(line), file);
  mlp->num_of_outputs = atoi(line);

  fgets(line, sizeof(line), file);
  mlp->num_of_inputs = atoi(line);

  fgets(line, sizeof(line), file);
  mlp->num_of_layers = atoi(line);

  mlp->layers = malloc(sizeof(Layer *) * mlp->num_of_layers);
  mlp->actfunc = malloc(sizeof(int) * mlp->num_of_layers);
  for (int i = 0; i < mlp->num_of_layers; i++) {
    mlp->layers[i] = malloc(sizeof(Layer));
    fgets(mlp->layers[i]->name, sizeof(mlp->layers[i]->name), file);
    mlp->layers[i]->name[strcspn(mlp->layers[i]->name, "\n")] = '\0';

    fgets(line, sizeof(line), file);
    mlp->layers[i]->num_of_neurons = atoi(line);
    fgets(line, sizeof(line), file);
    mlp->layers[i]->dim_of_neurons = atoi(line);
    fgets(line, sizeof(line), file);
    mlp->actfunc[i] = atoi(line);

    mlp->layers[i]->neurons =
        malloc(sizeof(Neuron *) * mlp->layers[i]->num_of_neurons);

    for (int j = 0; j < mlp->layers[i]->num_of_neurons; j++) {
      mlp->layers[i]->neurons[j] = malloc(sizeof(Neuron));

      fgets(mlp->layers[i]->neurons[j]->name,
            sizeof(mlp->layers[i]->neurons[j]->name), file);
      mlp->layers[i]
          ->neurons[j]
          ->name[strcspn(mlp->layers[i]->neurons[j]->name, "\n")] = '\0';

      fgets(line, sizeof(line), file);
      mlp->layers[i]->neurons[j]->dimension = atoi(line);

      fgets(line, sizeof(line), file);
      mlp->layers[i]->neurons[j]->bias = createNewValue(atof(line), "b");
      mlp->layers[i]->neurons[j]->bias->_isparameter = 1;

      mlp->layers[i]->neurons[j]->weights =
          malloc(sizeof(Value *) * mlp->layers[i]->neurons[j]->dimension);

      for (int k = 0; k < mlp->layers[i]->neurons[j]->dimension; k++) {
        fgets(line, sizeof(line), file);
        mlp->layers[i]->neurons[j]->weights[k] =
            createNewValue(atof(line), "w");
        snprintf(mlp->layers[i]->neurons[j]->weights[k]->name,
                 sizeof(mlp->layers[i]->neurons[j]->weights[k]->name), "w%d",
                 i);
        mlp->layers[i]->neurons[j]->weights[k]->_isparameter = 1;
      }
    }
  }
  fclose(file);
  return mlp;
}

typedef enum {
  MLP_DESCRIPTION,
  LAYER_DESCRIPTION,
  NEURON_DESCRIPTION
} parsing_state;
typedef struct {
  char *mlp_name, *layer_name, *neuron_name;
  size_t mlp_num_of_inputs, mlp_num_of_outputs, prev_layer_num_of_neurons,
      curr_layer_dim_of_neuron, curr_layer_num_of_neuron;
  parsing_state state;
} parsing_data;
int validate(const char *Fname) {
  FILE *file = fopen(Fname, "r");
  if (!file)
    return 1;
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);
  char *buf = malloc(sizeof(char) * size);
  if (!buf) {
    printf("unable to validate file %s ran out of memory\n", Fname);
    exit(EXIT_FAILURE);
  }
  size_t read = fread(buf, 1, size, file);
  fclose(file);
  buf[read] = '\0';

  // main validation starts from here
  parsing_data parsing;
  parsing.state = MLP_DESCRIPTION;
  parsing.mlp_name = NULL;
  parsing.layer_name = NULL;
  parsing.neuron_name = NULL;
  parsing.mlp_num_of_inputs = 0;
  parsing.mlp_num_of_outputs = 0;
  parsing.prev_layer_num_of_neurons = 0;
  parsing.curr_layer_dim_of_neuron = 0;
  parsing.prev_layer_num_of_neurons = 0;

  size_t i, line_start, line_end, line_len, line_num = 0;
  while (buf[i]) {
    line_num++;
    line_start = i;
    while (buf[i] && buf[i] != '\n')
      i++;
    line_end = i;
    line_len = line_start - line_end;
    char line[line_len + 1];
    for (size_t j = 0; j < line_len; j++)
      line[j] = buf[line_start + j];
    line[line_len] = '\0';

    switch (parsing.state) {
    case MLP_DESCRIPTION:
      if (!parsing.mlp_name) {
        if (line_len > 5) {
          printf("mlp name should be of 5 or less char\n");
          return 1;
        }
        parsing.mlp_name = line;
      }
      break;
    case LAYER_DESCRIPTION:
      break;
    case NEURON_DESCRIPTION:
      break;
    }
  }
  return 0;
}
