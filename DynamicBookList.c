// Write a dynamic array implementation that can grow and shrink as needed.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 40

char **allocateArray(size_t *size)
{
  char **arr = calloc(*size, sizeof(char *)); // rows: words
  for (size_t i = 0; i < *size; i++)          // columns: characters in words
  {
    *(arr + i) = calloc(MAX_LENGTH, sizeof(char));
  }
  return arr;
}

void addBook(char **arr, char *book, size_t i) // this is only addind one character, not one array of characters
{
  for (size_t j = 0; j < MAX_LENGTH; j++)
  {
    strncpy(*(arr + i), book, MAX_LENGTH - 1);
    *(*(arr + i) + (MAX_LENGTH - 1)) = '\0';
  }
};

void makeSpaceForNewBooks(size_t numOfBooks, char **arr, size_t *size)
{
  *arr = (char *)realloc(*arr, *size + numOfBooks);
  *size = *size + numOfBooks;
};

void deallocateSpaceForBooks(size_t numOfBooks, char **arr, size_t *size)
{
  *arr = (char *)realloc(*arr, *size - numOfBooks);
  *size = *size - numOfBooks;
};

void printArr(char **arr, size_t *size)
{
  printf("---\nThis is your list of books right now:\n");
  for (size_t i = 0; i < *size; i++)
  {
    printf(" - %s\n", *(arr + i));
  }
  printf("---\n");
}

int main()
{
  size_t *size = 0;
  char book[MAX_LENGTH] = "";

  printf("Enter a size for your list of books: ");
  scanf("%zu", size);

  char **arr = allocateArray(size);

  for (size_t i = 0; i < *size; i++)
  {
    printf("Enter a book's name for position %d: ", i);
    scanf("%s", book);

    if (strlen(book))
    {
      printf("Book added.\n");
    }
    else
    {
      printf("There was a problem with the input.");
    }

    addBook(arr, book, i);
  }

  printArr(&*arr, size);

  printf("Do you wish to edit your books list? (1 for yes and 0 for no): ");
  /*
  -- [?] this does not work idk why --

  char option = getchar();
  getchar(); // consume the \n left by the buffer [?]
  printf("Option: %c", option);
  */

  size_t option = 0;
  scanf("%zu", &option);

  switch (option)
  {
  case 1:
    printf("For add books, press 1, for remove books, press 0: ");
    scanf("%d", &option);
    switch (option)
    {
    case 1:
      size_t booksToAdd = 0;
      printf("How many new books will you enter?: ");
      scanf("%zu", &booksToAdd);
      makeSpaceForNewBooks(booksToAdd, arr, size);
      for (size_t i = 0; i <= booksToAdd; i++)
      {
        printf("Add the new book: ");
        scanf("%s", &book);

        addBook(arr, book, i);
      }
      printArr(*&arr, size);
      break;
    case 0:
      size_t booksToRemove = 0;
      printf("How many new books will you remove?: ");
      scanf("%zu", &booksToRemove);
      deallocateSpaceForBooks(booksToRemove, arr, size);
      break;
    default:
      break;
    }
    break;
  case 0:
    printf("Your list remains the same!\n");
    printArr(*&arr, size);
    break;
  default:
    break;
  }

  return 0;
}