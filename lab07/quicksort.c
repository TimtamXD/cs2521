#include <stdio.h>
#include <stdlib.h>

void quickSort(int items[],int l, int r);
void swap(int index1, int index2, int items[]);

int main(void){
    int size;
    int i;
    printf("Enter number of elements:");
    scanf("%d",&size);

    int * a = malloc(size * sizeof(int));   
    printf("Enter elements:\n"); 
    for(i=0;i<size;i++){
	    scanf("%d",&a[i]);
    }
   
    quickSort(a,0,size-1);
    
    printf("Sorted\n");
    for(i=0;i<size;i++){
            printf("%d ",a[i]);
    }
    printf("\n");

    return 0;
}

int partition(int a[], int l, int r);

void quickSort (int a[], int l, int r){         	
   int i;  
   if  (r <= l) {
       return;
   } 
   i = partition (a, l, r);  
   quickSort (a, l, i-1);  
   quickSort (a, i+1, r);
}

int partition (int a[], int l, int r) {   
   int i = l-1;
   int j = r;   
   int pivot = a[r]; //rightmost is pivot  	
   for(;;) {   
	while ( a[++i] < pivot) ;    
	while ( pivot <  a[--j] && j != l);
	if (i >= j) { 
      		break;
    	}    
	swap(i,j,a);  
    }
    //put pivot into place  
    swap(i,r,a);  
    return i; //Index of the pivot
}


void swap(int index1, int index2, int items[]){
    int tmp;
    tmp = items[index1];
    items[index1] = items[index2];
    items[index2] = tmp;
}

