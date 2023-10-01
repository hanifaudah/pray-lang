#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* get_file_contents(char *input_file) {
  FILE *f = fopen(input_file, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

  char *string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;
  return string;
}

char* handle_mem(int *mem, int *pntr, char *token) {
  char shift_left[] = "🙏";
  char shift_right[] = "🙏🙏";
  char increment[] = "🙏🙏🙏🙏";
  char decrement[] = "🙏🙏🙏🙏🙏";
  char input[] = "🙏🙏🙏🙏🙏🙏";
  char output[] = "🙏🙏🙏🙏🙏🙏🙏";
  // const int mem_length = 30000;
  char *compiled = "";
  if (strcmp(token, shift_left) == 0) {
    // (*pntr)--;
    // if (pntr < 0) {
    //   *pntr = mem_length;
    // }
    compiled = "pntr--;if(pntr < 0){pntr = mem_length - 1;}";
  } else if (strcmp(token, shift_right) == 0) {
    // (*pntr)++;
    // if (*pntr > mem_length) {
    //   *pntr = 0;
    // }
    compiled = "pntr++;if(pntr>=mem_length){pntr=0;}";
  } else if (strcmp(token, increment) == 0) {
    // mem[*pntr]++;
    compiled = "mem[pntr]++;";
  } else if (strcmp(token, decrement) == 0) {
    // mem[*pntr]--;
    compiled = "mem[pntr]--;";
  } else if (strcmp(token, input) == 0) {
    // char input;
    // scanf("%c", &input);
    // mem[*pntr] = (int) input;
    compiled = "char input;scanf(\"%c\", &input);mem[pntr] = (int) input;";
  } else if (strcmp(token, output) == 0) {
    // printf("%c", mem[*pntr]);
    compiled = "printf(\"%c\", mem[pntr]);";
  }
  return compiled;
}

int main(int argc, char *argv[]) {
  char *input_file = argv[1];
  char *output_file = argv[2];
  
  char *string = get_file_contents(input_file);

  char* token = strtok(string, " ");
  int mem[30000] = {0};
  int pntr = 0;
  char *start_loop = "🙏🙏🙏🙏🙏🙏🙏🙏";
  char *end_loop = "🙏🙏🙏🙏🙏🙏🙏🙏🙏";

  char compiled_file[12];
  sprintf(compiled_file, "%d.c", (int)time(NULL));

  FILE *fptr;
  fptr = fopen(compiled_file, "w");

  fprintf(fptr, "%s", "#include <stdio.h>\n#include <stdlib.h>\nint main(){int mem[30000]={0};int pntr = 0;const int mem_length = 30000;");
  
  while (token != NULL) {
    if (strcmp(token, start_loop) == 0) {
      char *loop_tokens[30000];
      int loop_tokens_idx = 0;
      char *loop_token = strtok(NULL, " ");
      while (loop_token != NULL && strcmp(loop_token, end_loop) != 0) {
        loop_tokens[loop_tokens_idx++] = loop_token;
        loop_token = strtok(NULL, " ");
      }
      fprintf(fptr, "%s", "while(mem[pntr]!=0){");
      for (int i = 0; i < loop_tokens_idx; i++) {
        char *tmp_compiled = handle_mem(mem, &pntr, loop_tokens[i]);
        fprintf(fptr, "%s", tmp_compiled);
      }
      fprintf(fptr, "%s", "}");
    } else {
      char *tmp_compiled = handle_mem(mem, &pntr, token);
      fprintf(fptr, "%s", tmp_compiled);
    }
    token = strtok(NULL, " ");
  }
  fprintf(fptr, "%s", "}");
  fclose(fptr);

  // for (int i = 0; i < 10; i++) {
  //   printf("%d ", mem[i]);
  // }

  char command[7 + 12 + strlen(output_file)];
  sprintf(command, "gcc -o %s %s", output_file, compiled_file);
  system(command);
  remove(compiled_file);
  return 0;
}