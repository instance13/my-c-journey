#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 40

char **allocateArray(size_t *size)
{
  char **arr = calloc(*size, sizeof(char *)); // rows: words
  for (size_t i = 0; i < *size; i++)
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

void makeSpaceForNewBooks(char ***arr, size_t *size, size_t numOfNewBooks)
{
  if (*size == 0)
  {
    *size = numOfNewBooks;
    *arr = allocateArray(size);
  }
  else
  {
    char **expandedList = (char **)realloc(*arr, (*size + numOfNewBooks) * sizeof(char *)); // i got a sigbus here 'cause i was reading garbage values

    for (size_t i = *size; i < (*size + numOfNewBooks); i++)
    {
      *(expandedList + i) = calloc(MAX_LENGTH, sizeof(char));
    }
    *arr = expandedList;
    *size = *size + numOfNewBooks;
  }
}

void deallocateSpaceForBooks(size_t numOfBooks, char ***arr, size_t *size)
{
  // Check: remove more books than exist
  if (numOfBooks > *size)
  {
    fprintf(stderr, "Cannot remove more books than exist.\n");
    return; // Exit the function if invalid input
  }

  // Free memory for the books being removed
  for (size_t i = *size - numOfBooks; i < *size; i++)
  {
    free(*(*(arr) + i)); // *(arr)[i]
  }

  // Reallocate to shrink the array
  char **shrunkArr = (char **)realloc(*arr, (*size - numOfBooks) * sizeof(char *));

  // Check if realloc was successful
  if (shrunkArr == NULL)
  {
    fprintf(stderr, "Memory allocation failed!\n");
    return; // Early return to leave the original array unchanged
  }

  *arr = shrunkArr; // Update the original pointer to the shrunk array
  *size = *size - numOfBooks;
}

void removeFromIndex(char ***arr, size_t *size, int i)
{
  if (arr == NULL)
  {
    fprintf(stderr, "You cannot delete from an empty list.\n");
  }
  if (*size - 1 == 0)
  { // check if the only element in the array is being deleted

    deallocateSpaceForBooks(1, arr, size);
    *size = 0;
    printf("Now you have an empty book list.\n");
  }
}

void removeFromName() {}

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
  // size_t *size = 0; this sets the pointer to null and lead to segmentation fault errors later.
  size_t *size = calloc(1, sizeof(size_t));
  char book[MAX_LENGTH] = "";

  printf("Enter a size for your list of books: ");
  scanf("%zu", size);

  char **arr = allocateArray(size);

  for (size_t i = 0; i < *size; i++)
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

  if (option == 1)
  {
    int subOption = 0;
    do
    {
      printf("---\n1. Add books to the top.\n2. Remove books from the top.\n3. Remove a specific book by index.\n4. Remove a specific book by name.\n5. Exit editing.\n---\n");
      scanf("%d", &subOption);
      switch (subOption)
      {
      case 1:
      {
        size_t booksToAdd = 0;
        printf("How many new books will you enter?: ");
        scanf("%zu", &booksToAdd);

        // Expand the array and update arr
        makeSpaceForNewBooks(&arr, size, booksToAdd);

        // Input new book names
        for (char **p = arr + (*size - booksToAdd); p < arr + *size; p++)
        {
          printf("Add the new book: ");
          scanf("%s", book);           // Read the new book name safely
          addBook(arr, book, p - arr); // Add at new positions using pointer arithmetic
        }

        printArr(arr, size); // Print updated array after adding books
        break;
      }
      case 2:
      {
        size_t numOfBooks = 0;
        printf("How many books do you want to remove from the top?: ");
        scanf("%zu", &numOfBooks);

        deallocateSpaceForBooks(numOfBooks, &arr, size);

        printArr(arr, size);

        break;
      }
      case 3:
      {
        int i = 0;
        printf("Enter the index of the book you want to delete (starting at 0; 0 is the first element): ");
        scanf("%d", &i);
        removeFromIndex(&arr, size, i);
        break;
      }
      case 4:
      {
        break;
      }
      case 5:
      {
        printf("You finished editing your book list.\n");
        printArr(arr, size);
        break;
      }
      }
    } while (subOption != 5);
  }
  else
  {
    printf("Your list remains the same!\n");
    printArr(arr, size);
  }
}
