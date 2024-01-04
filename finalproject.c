#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_KEY_LENGTH 2 


// have a helper function that transforms each hex char into binary
//struct KeyValuePair { char key[MAX_KEY_LENGTH];  const int value;};
struct KeyValuePair { char key[MAX_KEY_LENGTH];  char value[5];};
struct KeyValuePair dictionary[] = {
    {"0", "0000"}, {"1", "0001"}, {"2", "0010"}, {"3","0011"}, {"4", "0100"},  {"5", "0101"},
    {"6", "0110"}, {"7", "0111"},{"8", "1000"}, {"9", "1001"}, {"A", "1010"}, {"B", "1011"},
    {"C", "1100"}, {"D", "1101"}, {"E", "1110"}, {"F", "1111"}};

int my_utf8_strcmp(unsigned char *string1, unsigned char *string2);
const char* getBinaryValue(const char key) {
    for (int i = 0; i < 16; i++) { // iterate through alll dict keys
        if (my_utf8_strcmp((unsigned char *)dictionary[i].key, (unsigned char *)&key) == 0) {
            return dictionary[i].value;
        }}
    return "not found";
}
void concatenateStrings(char* dest, const char* src) {
    while (*dest != '\0') {
        dest++; // point dest to end of dest string
    }
    while (*src != '\0') {
        *dest = *src; // copy src char to end of dest
        dest++;       //increment dest
        src++;        //increment src
    }
    *dest = '\0'; // add null terminator
}

//Validates that the input string is a valid UTF8 encoded string.
int my_utf8_check(unsigned char *string){
  //check that is has the proper unicode template
  //input has to start with \U and have a digit after it
  if(string[0] != 'U'  || string[1] != '/' || string[2] == '\0' ){
    printf("invalid input, didnt pass UTF8 check\n");
    return 0;}
  return 1;
}

//: Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string){
int count = 0;
    while (*string != '\0') {
        count++; //increment count
        string++; //move to next char
    }
    return count;
}

//: Returns the UTF8 encoded character at the location specified. 
//If the input string is improperly encoded, this function should return NULL to indicate an error.
char *my_utf8_charat(unsigned char *string, int index){
      if (index < 0 || string[index] == '\0') {return NULL;}// index out of bounds, index > str

      char *result = (char *)malloc(5); // Aassign space for 4 bytes and null terminator
      if (result == NULL) {printf("Memory allocation failed.\n");return NULL;} 
      
      int i = index;
      int byteCount = 0;
      while (i >= 0 && ((string[i] & 0xC0) == 0x80)) { //use hexa code to check if the char is a continuation byte
                                                       // bitwise and with 11000000 is equal to 10000000?
          i--; byteCount++; 
      }

      if (byteCount == 0) {
          result[0] = string[index];
          result[1] = '\0'; 
      } else {
          for (int j = 0; j <= byteCount; j++) {
              result[j] = string[index - byteCount + j];
          }
          result[byteCount + 1] = '\0'; //add null terminator
      }
      return result;
}

char* bitwiseOR(const char* str1, const char* str2, int startIndex, int length) {
  //printf("strat index %d\n", startIndex);
  // Convert negative index to positive index if provided
  if (startIndex < 0) {
    startIndex = (int)(my_utf8_strlen((unsigned char *)str1)) + startIndex;
  }
  //allocate unknown size 
  size_t len1 = my_utf8_strlen((unsigned char *)str1);
  size_t len2 = my_utf8_strlen((unsigned char *)str2);
  
  // Validate the range of indexes for str1
  if (startIndex < 0 || (size_t)startIndex >= len1 || (size_t)(startIndex + length) > len1) {
      //printf("%d, %d, %d\n", startIndex < 0 , (size_t)startIndex >= len1 , (size_t)(startIndex + length) > len1);
      printf("Invalid index range for str1 \n");return NULL;}
  

  size_t maxLength = (len2 > len1 - startIndex) ? len1 - startIndex : len2;
  char* result = (char*)malloc(maxLength + 1); // Allocate memory for the result string
  if (result == NULL) {printf("Memory allocation failed.\n");return NULL;}

  // Perform bitwise OR operation between specified bit ranges of str1 and str2
  for (size_t i = 0; i < maxLength; i++) {
    int char1 = (startIndex + i < len1) ? (str1[startIndex + i] - '0') : 0;
    int char2 = str2[i] - '0';
      //printf("char1 char2 %d %d\n", char1, char2);

      // Perform bitwise OR operation and store the result as a character in the result string
      result[i] = (char)(char1 | char2) + '0'; // Convert integer back to character
    }
    result[maxLength] = '\0'; // Add null terminator to the result string
    printf("result: %s\n", result);
    return result;
}

char* binaryToHex(const char* binaryInput) {
    size_t binaryLength = strlen(binaryInput);
    size_t hexOutputLength = (binaryLength + 3) / 4 + 1;

    char* hexOutput = (char*)malloc(hexOutputLength * sizeof(char));
    if (hexOutput == NULL) { printf("Memory allocation failed.\n"); return NULL;}

    int hexIndex = 0;
    int binaryIndex = binaryLength - 1;

    while (binaryIndex >= 0) {
        int value = 0;
        int bitCount = 0;

        while (bitCount < 4 && binaryIndex >= 0) {
            value += (binaryInput[binaryIndex] - '0') << bitCount;
            bitCount++;
            binaryIndex--;
        }

        if (value < 10) {
            hexOutput[hexIndex++] = value + '0';
        } else {
            hexOutput[hexIndex++] = value - 10 + 'A';
        }
    }

    hexOutput[hexIndex] = '\0';

    // Reverse the hexOutput string since we filled it from right to left
    int start = 0;
    int end = hexIndex - 1;
    while (start < end) {
        char temp = hexOutput[start];
        hexOutput[start] = hexOutput[end];
        hexOutput[end] = temp;
        start++;
        end--;
    }

    return hexOutput;
}

void unicodeToBinary(unsigned char *input, char *binaryOutput) {
    char *binaryOutputPtr = binaryOutput; // Pointer
    if (*input == 'U' && (( *(input + 1) == '/'))){
    printf("input before removing U: %s\n", input);
    input += 2;
    printf("input after removin U: %s\n", input);
    }

    for (int i = 0; i < my_utf8_strlen(input); i++) {
        const char *binaryValue = getBinaryValue(input[i]);
        //printf("ascii, binary %c %s\n", input[i], binaryValue);

        while (*binaryValue != '\0') {
            *binaryOutputPtr = *binaryValue; // copying each character
            binaryOutputPtr++; // move the pointer to the next position
            binaryValue++; // move to the next character in binaryValue
        }
    }
    *binaryOutputPtr = '\0'; // Add null terminator to the end of binaryOutput
}

//u notation to hex utf8
// U/0024  --> 24
// U/10348 --> F0908D88
char *my_utf8_encode(unsigned char *input, unsigned char *output){
  //make sure its a proper utf8
  int ans = my_utf8_check(input); if (ans ==0){return 0;}

  
  //convert unicode to binary
  char binaryOutput[100] = ""; // array that saves new binary string
  unicodeToBinary(input, binaryOutput); // Call the helper function
  printf("input converted to binary: %s\n", binaryOutput);

  //count significant bits of s
  // start on the [i] position on s, i=0, i++, i<= len(s), and once s[i]==1, return the pos if i. the amount of significant bits will be strlen(s)-i
  int significant = 0;
  for (int i = 0; i < my_utf8_strlen((unsigned char *)binaryOutput); i++) {
      if (binaryOutput[i] == '1') {
          significant = my_utf8_strlen((unsigned char *)binaryOutput) - i;
          break;
      }}

  //select sixe of utf=8 based on number of significant bits
  // map into utf-8
  // use bitwise or to set digits 
  printf("significant: %d\n", significant);
  char *result= NULL;
  if (significant <=7) {  
    printf("using 1 byte utf-8\n");
    // concatenate the last 7 chars of binaryOutput to '0' (UTF8 identifier)
    result = (char *)malloc(8); // Allocate memory for the result (8 bytes including '\0')
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    result[0] = '0'; // Set the first character as '0'
    char *conc = binaryOutput + strlen(binaryOutput) - 7; // Pointer to the last 'x' char
    concatenateStrings(result +1, conc);
    printf("result --> %s\n", result);  
  }
  else if (significant <=11){
    printf("using 2 byte utf-8\n");
    char* result1  = bitwiseOR(binaryOutput, "00000", -11, 5); //use or to isolate desired range
    char* result2 = bitwiseOR(binaryOutput, "000000", -6, 6);
    
    result = (char *)malloc(17);
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    result[0] = '1'; result[1] = '1'; result[2] = '0'; // Set unicode template
    
    concatenateStrings(result +1, result1);
    result[8] = '1'; result[9] = '0'; //unicode template
    concatenateStrings(result +1, result2);
    printf("result --> %s\n", result);  
    free(result1); free(result2);
  }
  else if (significant <=16){
    printf("using 3 byte utf-8\n");
    char* result1  = bitwiseOR(binaryOutput, "0000", -16, 4);
    char* result2 = bitwiseOR(binaryOutput, "000000", -12, 6); 
    char* result3 = bitwiseOR(binaryOutput, "000000", -6, 6);

    result = (char *)malloc(25);
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    result[0] = '1'; result[1] = '1'; result[2] = '1';  result[3] = '0';// Set unicode template
    concatenateStrings(result +1, result1);
    result[8] = '1'; result[9] = '0'; //unicode template
    concatenateStrings(result +9, result2);
    result[16] = '1'; result[17] = '0'; //unicode template]
    concatenateStrings(result +17, result3);
    printf("result --> %s\n", result);  
    free(result1); free(result2); free(result3);
    
  }
  else if (significant <=21){
    printf("using 4 byte utf-8\n");
    char* result1 = bitwiseOR(binaryOutput, "000", -20, 3);
    char* result2 = bitwiseOR(binaryOutput, "000000", -18, 6);
    char* result3 = bitwiseOR(binaryOutput, "000000", -12, 6);
    char* result4  = bitwiseOR(binaryOutput, "000000", -6, 6);
    
    result = (char *)malloc(33);
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    result[0] = '1'; result[1] = '1'; result[2] = '1';  result[3] = '1'; result[4] = '0';// Set unicode template
    concatenateStrings(result +1, result1);
    result[8] = '1'; result[9] = '0'; //unicode template
    concatenateStrings(result +9, result2);
    result[16] = '1'; result[17] = '0'; //unicode template]
    concatenateStrings(result +17, result3);
    result[24] = '1'; result[25] = '0'; //unicode template]
    concatenateStrings(result +25, result4);
    printf("result --> %s\n", result);  
    free(result1); free(result2); free(result3); free(result4);
  }
 
  // if its too long for utf-8
  if (result == NULL){printf("fail, str too long");}
  else{
    printf("binary result --> %s\n", result);
    char* hexResult = binaryToHex(result);
    if (hexResult != NULL) {
      printf("Hexadecimal representation: %s\n", hexResult);
      
      // Copy the hexResult to output parameter using pointers
      unsigned char* outputPtr = (unsigned char*)output;
      char* hexResultPtr = hexResult;
      while (*hexResultPtr != '\0') {
          *outputPtr = *hexResultPtr;
          outputPtr++;
          hexResultPtr++;
      }
      *outputPtr = '\0'; // Add null terminator
      //free(hexResult); // Free dynamic memory 
      //free(result);
  }
  }
  return result;
}

//hex utf8 notation to u
//F0908D88 --> U/10348
char *my_utf8_decode(unsigned char *input, unsigned char *output){
  // transform hex to binary
  //convert unicode to binary
  char binaryOutput[100] = ""; // array that saves new binary string
  unicodeToBinary(input, binaryOutput); // Call the helper function
  printf("binary utf8 %s\n", binaryOutput);

  //use biwise or to isolate characters from utf8 template
  //check len on binary utf8, decode acordingly
  char *result= NULL;
  if (my_utf8_strlen((unsigned char*)binaryOutput)==8){
    printf("decoding one byte utf-8\n");
    result  = bitwiseOR(binaryOutput, "0000000", -7, 7); //use or to isolate desired range
    }

  else if(my_utf8_strlen((unsigned char*)binaryOutput)==16){
    printf("orig result before assign: %s\n", result);
    char* result1 = bitwiseOR(binaryOutput, "000000", -16, 4);
    char* result2 = bitwiseOR(binaryOutput, "000000", -6, 6);

    result = (char *)malloc(17);
    printf("result after malloc: %s\n", result);
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    printf("concatenating res, res1 %s %s\n", result, result1);
    concatenateStrings(result , result1);
    printf("concatenating res, rs2 %s %s\n", result, result2);
    concatenateStrings(result , result2);
    }

  else if(my_utf8_strlen((unsigned char*)binaryOutput)==24){
    printf("OPTION 3 orig result before assign: %s\n", result);
    char* result1  = bitwiseOR(binaryOutput, "0000", -20, 4); //use or to isolate desired range
    char* result2  = bitwiseOR(binaryOutput, "000000", -14, 6); //use or to isolate desired range
    char* result3 = bitwiseOR(binaryOutput, "000000", -6, 6);

    result = (char *)malloc(25);
    printf("result after malloc: %s\n", result);
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    printf("concatenating res, res1 %s %s\n", result, result1);
    concatenateStrings(result , result1);
    printf("concatenating res, rs2 %s %s\n", result, result2);
    concatenateStrings(result , result2);
    printf("concatenating res, rs3 %s %s\n", result, result3);
    concatenateStrings(result, result3);
    }

  else if(my_utf8_strlen((unsigned char*)binaryOutput)==32){
    printf("OPTION 4 orig result before assign: %s\n", result);
    char* result1 = bitwiseOR(binaryOutput, "000",    -27, 3); //use or to isolate desired range
    char* result2 = bitwiseOR(binaryOutput, "000000", -22, 6); //use or to isolate desired range
    char* result3 = bitwiseOR(binaryOutput, "000000", -14, 6);
    char* result4 = bitwiseOR(binaryOutput, "000000", -6,  6);

    result = (char *)malloc(33);
    printf("result after malloc: %s\n", result);
    if (result == NULL) {printf("Memory allocation failed.\n");return -1;} // Return an error code 
    printf("concatenating res, res1 %s %s\n", result, result1);
    concatenateStrings(result , result1);
    printf("concatenating res, rs2 %s %s\n", result, result2);
    concatenateStrings(result , result2);
    printf("concatenating res, rs3 %s %s\n", result, result3);
    concatenateStrings(result, result3);
    printf("concatenating res, rs4 %s %s\n", result, result4);
    concatenateStrings(result, result4);
    }
  //convert binary utf into character
  // if its too long for utf-8
  if (result == NULL){printf("fail, str too long");}

  else{
    printf("binary result --> %s\n", result);
    char* hexResult = binaryToHex(result);
        if (hexResult != NULL) {
            char temp[] = "\\U";
            concatenateStrings(temp, hexResult);
            hexResult = temp;
            printf("Hexadecimal representation: %s\n", hexResult);

          // Copy the hexResult to output parameter using pointers
          unsigned char* outputPtr = (unsigned char*)output;
          char* hexResultPtr = hexResult;
          while (*hexResultPtr != '\0') {
              *outputPtr = *hexResultPtr;
              outputPtr++;
              hexResultPtr++;
          }
          *outputPtr = '\0'; // null terminator
          
        // Free dynami memory 
        //free(result);
      }
  }
  return result;
  
}

//: Returns whether the two strings are the same (similar result set to strcmp() )
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    while (*string1 && *string2) {
        // Compare the current characters in string1 and string2
        if (*string1 != *string2) {
            int dif = *string1 - *string2; // Return the difference
            if(dif>0){return 1;}else{return -1;}
        }
        string1++;
        string2++;
    }

    // If one string ended, check the lengths to determine the result
    if (*string1 == '\0' && *string2 == '\0') {
        return 0; // Strings are equal
    } else {
      int dif = *string1 - *string2; // Return the difference
      if(dif>0){return 1;}else{return -1;} // Return the difference in lengths
    }
}


int test(void) {

  //test that finctions work correctly
  printf("****** testing mu_utf8_encode ******\n");

  unsigned char *unicodeInput = (unsigned char *)"U/0024"; // Example Unicode input
  unsigned char *out          = (unsigned char *)"24";
  unsigned char output[100]; // Buffer to store the output
  char* my_encoded = my_utf8_encode(unicodeInput, output);
  if (my_utf8_strcmp(my_encoded, out)==0){printf("encoding test passed for one byte utf8\n"); }
  else{printf("encoding test not passed for one byte utf8\n");}

  unsigned char *unicodeInput2 = (unsigned char *)"U/00A3"; // Example Unicode input
  unsigned char *out2          = (unsigned char *)"C2A3";
  unsigned char output2[100]; // Buffer to store the output
  char* my_encoded2 = my_utf8_encode(unicodeInput2, output2);
  if (my_utf8_strcmp(my_encoded2, out2)==0){printf("encoding test passed for two byte utf8\n"); }
  else{printf("encoding test not passed for two byte utf8\n");}

  unsigned char *unicodeInput3 = (unsigned char *)"U/0939"; // Example Unicode input
  unsigned char *out3          = (unsigned char *)"E0A4B9";
  unsigned char output3[100]; // Buffer to store the output
  char* my_encoded3 = my_utf8_encode(unicodeInput3, output3);
  if (my_utf8_strcmp(my_encoded3, out3)==0){printf("encoding test passed for three byte utf8\n"); }
  else{printf("encoding test not passed for three byte utf8\n");}

  unsigned char *unicodeInput4 = (unsigned char *)"U/0024"; // Example Unicode input
  unsigned char *out4          = (unsigned char *)"24";
  unsigned char output4[100]; // Buffer to store the output
  char* my_encoded4 = my_utf8_encode(unicodeInput4, output4);
  if (my_utf8_strcmp(my_encoded4, out4)==0){printf("encoding test passed for four byte utf8\n"); }
  else{printf("encoding test not passed for four byte utf8\n");}

  printf("****** testing mu_utf8_decode ******\n");
  
  unsigned char *hexutf8 = (unsigned char *)"24"; // Example Unicode input
  unsigned char *outp    = (unsigned char *)"U/24";
  unsigned char outputB[100]; // Buffer to store the output
  char* my_decoded = my_utf8_decode(hexutf8, outp);
  if (my_utf8_strcmp(my_encoded, out)==0){printf("decoding test passed for one byte utf8\n"); }
  else{printf("decoding test not passed for one byte utf8\n");}

  unsigned char *hexutf82 = (unsigned char *)"C2A3"; // Example Unicode input
  unsigned char *outp2    = (unsigned char *)"U/A3";
  unsigned char outputB2[100]; // Buffer to store the output
  char *my_decoded2 = my_utf8_decode(hexutf82, outp2);
  if (my_utf8_strcmp(my_encoded2, out2)==0){printf("decoding test passed for two byte utf8\n"); }
  else{printf("decoding test not passed for two byte utf8\n");}

  unsigned char *hexutf83 = (unsigned char *)"E0A4B9"; // Example Unicode input
  unsigned char *outp3    = (unsigned char *)"U/939";
  unsigned char outputB3[100]; // Buffer to store the output
  char* my_decoded3 = my_utf8_decode(hexutf83, outp3);
  if (my_utf8_strcmp(my_encoded3, out3)==0){printf("decoding test passed for three byte utf8\n"); }
  else{printf("decoding test not passed for three byte utf8\n");}

  unsigned char *hexutf84 = (unsigned char *)"F0908D88"; // Example Unicode input
  unsigned char *outp4    = (unsigned char *)"U/10348";
  unsigned char outputB4[100]; // Buffer to store the output
  char* my_decoded4 = my_utf8_decode(hexutf84, outp4);
  if (my_utf8_strcmp(my_encoded4, out4)==0){printf("decoding test passed for four byte utf8\n"); }
  else{printf("decoding test not passed for four byte utf8\n");}

  printf("****** testing my_utf8_charat ******\n");
  unsigned char *temp = (unsigned char *)"F0908D88"; 

  unsigned char my_charat = my_utf8_charat(temp, 0);
  if (my_charat == 'F'){printf("charat test passed\n");}
  
  return 0;
}

int main(void){

  //encode
  unsigned char *unicodeInput = (unsigned char *)"U/0024"; // Example Unicode input
  unsigned char output[100]; // Buffer to store the output
  char *encoded = my_utf8_encode(unicodeInput, output);

  if (encoded != NULL){
    printf("encoded: %s\n", encoded);
    free(encoded);}
  else{
    printf("failure to complete my_utf8_encode\n" );}

  //decode
  unsigned char *hexutf8 = (unsigned char *)"24"; // Example Unicode input
  unsigned char output2[100]; // Buffer to store the output
  char *decoded = my_utf8_decode(hexutf8, output2);

  if (decoded != NULL){
    printf("decoded: %s\n", encoded);
    free(decoded);}
  else{
    printf("failure to complete my_utf8_decode\n" );}
  
}