void dummy_test_entrypoint() {

}

void main() {
  //Create pointer to a char and point it to the first text cell of the video memory
  char* video_memory = (char*) 0xb8000;
  //At the address pointed to by video_memory, store the character 'X'
  *video_memory = 'H';
}
