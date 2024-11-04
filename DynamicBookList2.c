#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 40

char **allocateArray(size_t size)
{
  char **arr = calloc(size, sizeof(char *)); // rows: words
  for (size_t i = 0; i < size; i++)
  { // columns: characters in words
    *(arr + i) = calloc(MAX_LENGTH, sizeof(char));
  }
  return arr;
}

void addBook(char **arr, char *book, size_t i)
{
  strncpy(*(arr + i), book, MAX_LENGTH - 1);
  *(*(arr + i) + (MAX_LENGTH - 1)) = '\0'; // Ensure null termination
}

char **makeSpaceForNewBooks(size_t numOfBooks, char **arr, size_t size)
{
  size_t newSize = size + numOfBooks;
  char **expandedArr = (char **)realloc(arr, newSize * sizeof(char *));

  if (expandedArr == NULL)
  {
    fprintf(stderr, "Memory allocation failed!\n");
    return NULL; // or handle error as needed
  }

  for (char **p = expandedArr + size; p < (expandedArr + newSize); p++)
  {
    *p = calloc(MAX_LENGTH, sizeof(char)); // Allocate space for new book names
  }

  for (char **p = expandedArr; p < (expandedArr + size); p++)
  {
    strncpy(*(p + size), *(p), MAX_LENGTH - 1);     // Copy existing book names
    *(*(p + size) + (MAX_LENGTH - 1)) = '\0';       // Ensure null termination
  }
  return expandedArr;
}

char **deallocateSpaceForBooks(size_t numOfBooks, char **arr, size_t size)
{
  // Check if we're trying to remove more books than exist
  if (numOfBooks > size)
  {
    fprintf(stderr, "Cannot remove more books than exist.\n");
    return arr; // Return original array without changes
  }

  // Free memory for the books being removed
  for (size_t i = size - numOfBooks; i < size; i++)
  {
    free(*(arr + i)); // Free each book's memory
  }

  // Reallocate to shrink the array
  char **shrunkArr = (char **)realloc(arr, (size - numOfBooks) * sizeof(char *));

  // Check if realloc was successful
  if (shrunkArr == NULL)
  {
    fprintf(stderr, "Memory allocation failed!\n");
    return arr; // Return original array if realloc fails
  }

  return shrunkArr; // Return the updated pointer
}

char **removeFromIndex(char **arr, size_t size, int i) {
  if (arr == NULL)
  {
    fprintf(stderr, "You cannot delete from an empty list.\n");
  }
  if (size-1 == i) { // check if the only element in the array is being deleted
    char **newArr = deallocateSpaceForBooks(1, arr, size);
    printf("Now you have an empty book list.");
    return newArr;
  }
}

char **removeFromName() {}

void printArr(char **arr, size_t size)
{
  printf("---\nThis is your list of books right now:\n");
  for (size_t i = 0; i < size; i++)
  {
    printf(" - %s\n", *(arr + i));
  }
  printf("---\n");
}

int main()
{
  size_t size = 0;
  char book[MAX_LENGTH] = "";

  printf("Enter a size for your list of books: ");
  scanf("%zu", &size);

  char **arr = allocateArray(size);

  for (size_t i = 0; i < size; i++)
  {
    printf("Enter a book's name for position %zu: ", i);
    scanf("%s", book);

    if (strlen(book))
    {
      addBook(arr, book, i);
      printf("Book added.\n");
    }
    else
    {
      printf("There was a problem with the input.\n");
    }
  }

  printArr(arr, size);

  printf("Do you wish to edit your books list? (1 for yes and 0 for no): ");

  int option;
  scanf("%d", &option);

  switch (option)
  {
  case 1:
    printf("---\n1. Add books to the top.\n2. Remove books from the top.\n3. Remove a specific book by index.\n4. Remove a specific book by name.\n---\n");
    scanf("%d", &option);
    switch (option)
    {
    case 1:
    {
      size_t booksToAdd = 0;
      printf("How many new books will you enter?: ");
      scanf("%zu", &booksToAdd);

      // Expand the array and update arr
      char **expandedArr = makeSpaceForNewBooks(booksToAdd, arr, size);
      if (expandedArr == NULL)
      {
        fprintf(stderr, "Memory allocation failed!\n");
        break;
      }

      arr = expandedArr;
      size += booksToAdd;

      // Input new book names
      for (char **p = arr + (size - booksToAdd); p < arr + size; p++)
      {
        printf("Add the new book: ");
        scanf("%s", book);           // Read the new book name safely
        addBook(arr, book, p - arr); // Add at new positions using pointer arithmetic
      }

      printArr(arr, size); // Print updated array after adding books
      break;
    }
    case 2:
      size_t numOfBooks = 0;
      printf("How many books do you want to remove from the top?: ");
      scanf("%zu", &numOfBooks);
      char **newBookList = deallocateSpaceForBooks(numOfBooks, arr, size);
      arr = newBookList;
      size = size - numOfBooks;
      printArr(arr, size);
      break;
    case 3:
      int i = 0;
      printf("Enter the index of the book you want to delete (starting at 0; 0 is the first element): ");
      scanf("%d", &i);
      removeFromIndex(arr, size, i);
      break;
    case 4:
      break;
    }
    break;
  case 0:
    printf("Your list remains the same!\n");
    printArr(arr, size);
    break;
  default:
    break;
  }
}