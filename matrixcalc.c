#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

pthread_mutex_t lockcreator;
pthread_mutex_t locksum;
int a[20][20] ={'\0'};
int count; 

void *createMatrix() //matrixi oluşturan fonksiyon
{
	for(int c=1; c<=count; c++) //kullanıcının girdiği sayı kadar dönülür
	{
		pthread_mutex_lock(&lockcreator);//kendini kitler
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++){
				int randomNumber = 1+rand()%99; //1-100 arasında rastgele sayı üretir
				a[i][j] = randomNumber; //dizinin ilgili elemanına rastgele üretilen sayı atanır
			}
		}
		printMatrix(a);//matrixi konsola basan fonskiyon çağırılır
		pthread_mutex_unlock(&locksum);//sumMmatrix fonksiyonunun kilidini açar
	}
}

void sumMatrix()//matrixi toplayan fonskiyon
{
	for(int c=1; c<=count; c++)//kullanıcının girdiği sayı kadar dönülür
	{
		pthread_mutex_lock(&locksum);//kendini kitler
		int sum = 0;
		for (int m = 0; m < 20; m++)
		{
			for (int n = 0; n < 20; n++){
				sum+=a[m][n];//toplama işlemi
			}
		}
		printf("\nToplam = %d",sum);//sonucu konsola basar
		printf("---------------------------------------------\n");
		pthread_mutex_unlock(&lockcreator);//createMatrix fonksiyonunun kilidini açar
	}
}

void printMatrix(int a[20][20])//matrixi konsola basar
{
	printf("\n");
	for (int k = 0; k < 20; k++)
	{
		for (int l = 0; l < 20; l++){
		    if(a[k][l] > 9)
		    	printf("%d ",a[k][l]);
		    else
		    	printf(" %d ",a[k][l]);
		}
		printf("\n");
	}
}

void ctrl_c_handler(int dummy)//ctrl+c kombinasyonunu algılayan fonskiyon
{
	printf("CTRL+C BASILDI\n");
	signal(SIGINT, ctrl_c_handler);
	pthread_mutex_destroy(&lockcreator); //thread yok eder
	pthread_mutex_destroy(&locksum);//thread yok eder
	exit(0);//cıkış
}

void ctrl_z_handler(int dummy)//ctrl+z kombinasyonunu algılayan fonskiyon
{
	printf("CTRL+Z BASILDI\n");
	signal(SIGTSTP, ctrl_z_handler);
	pthread_mutex_destroy(&lockcreator);//thread yok eder
	pthread_mutex_destroy(&locksum);//thread yok eder
	exit(0);//cıkış
}


int main()
{
	pthread_mutex_init(&lockcreator, NULL);
	pthread_mutex_init(&locksum, NULL);
	pthread_mutex_lock(&locksum);//sumMatrix fonskiyonunu kitler
    
   	pthread_t thread_matrix_creator;
   	pthread_t thread_matrix_sum;
    
    while(1)
    {
		signal(SIGINT, ctrl_c_handler);//sinyal kontrol
		signal(SIGTSTP, ctrl_z_handler);//sinyal kontrol

    	printf("\nKaç matrix oluşturulsun: ");
		scanf("%d",&count);

		if(count <= 0) //0 veya negatif bir sayı girildiğinde hata verir ve threadleri yok ederek programdan çıkar
		{
			printf("yanlış bir değer girdiniz\n");
			pthread_mutex_destroy(&lockcreator);//thread yok eder
			pthread_mutex_destroy(&locksum);//thread yok eder
			exit(0);//cıkış
		}
		
		pthread_create(&thread_matrix_creator, NULL, createMatrix, NULL);//thread oluşumu
		pthread_create(&thread_matrix_sum, NULL, sumMatrix, NULL);//thread oluşumu

		pthread_join(thread_matrix_creator, NULL);
		pthread_join(thread_matrix_sum, NULL);

    }
    return 0;
}
