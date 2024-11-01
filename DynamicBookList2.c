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

  for (char **p = expandedArr + size; p < expandedArr + newSize; p++)
  {
    *p = calloc(MAX_LENGTH, sizeof(char)); // Allocate space for new book names
  }

  for (char **p = expandedArr; p < expandedArr + size; p++)
  {
    *(p + size) = calloc(MAX_LENGTH, sizeof(char)); // Allocate space for each book
    strncpy(*(p + size), *(p), MAX_LENGTH - 1);     // Copy existing book names
    *(*(p + size) + (MAX_LENGTH - 1)) = '\0';       // Ensure null termination
  }
  return expandedArr;
}

char **deallocateSpaceForBooks(size_t numOfBooks, char **arr, size_t size)
{
  return (char **)realloc(*arr, (size - numOfBooks) * sizeof(char *));
}

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
    printf("For add books, press 1, for remove books, press 0: ");
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
    case 0:
      printf("Remove books. ");
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