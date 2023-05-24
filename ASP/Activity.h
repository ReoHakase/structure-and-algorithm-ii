typedef struct
{
  char *name;
  unsigned start;
  unsigned finish;
} ACT;

void sortActivity(ACT act[], unsigned n);
void swapActivity(ACT act[], unsigned i, unsigned j);
unsigned findIdealPivotIndex(ACT act[], unsigned n);
void printActivity(ACT act[], unsigned n);