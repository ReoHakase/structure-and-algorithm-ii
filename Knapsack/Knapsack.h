typedef struct
{
  char *name;
  float weight;
  float value;
  float performance;
} OBJ;

void sortObject(OBJ obj[], unsigned n);
unsigned findIdealPivotIndex(OBJ obj[], unsigned n);
void swapObject(OBJ obj[], unsigned i, unsigned j);
void printObject(OBJ obj[], unsigned n);