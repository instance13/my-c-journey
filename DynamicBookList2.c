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
  for (size_t i = 0; i < size; i++)
  {
    *(expandedArr + i) = strdup(*(arr+i));
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
  size_t size = 0; // Changed to a regular variable
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

      char **expandedArr = makeSpaceForNewBooks(booksToAdd, arr, size);

      // printArr(*&arr, size + booksToAdd);

      for (size_t i = size; i < (booksToAdd + size); i++)
      {
        printf("Add the new book: ");
        scanf("%s", book);             // Read the new book name
        addBook(expandedArr, book, i); // Add at new positions
      }
      // printArr(*&arr, size + booksToAdd); // this is not executing right now
      break;
    }
    case 0:
      size_t booksToRemove = 0;
      printf("How many books will you remove?: ");
      scanf("%zu", &booksToRemove);
      deallocateSpaceForBooks(booksToRemove, arr, size);
      break;
    default:
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