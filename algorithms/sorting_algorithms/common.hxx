
template<typename T>
void swap(T* a, T* b) {
    /*
        swapping two values by reference
    */
   T temp = *a;
   *a = *b;
   *b = temp;
}
