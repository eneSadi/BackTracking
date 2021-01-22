#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//girilen rengi sayiya cevirir
int color2number(char color[10]){
	
	if(strcmp(color, "red") == 0){
		return 0;
	}
	if(strcmp(color, "green") == 0){
		return 1;
	}
	if(strcmp(color, "blue") == 0){
		return 2;
	}
	if(strcmp(color, "yellow") == 0){
		return 3;
	}
	if(strcmp(color, "purple") == 0){
		return 4;
	}
	if(strcmp(color, "black") == 0){
		return 5;
	}
	if(strcmp(color, "white") == 0){
		return 6;
	}
	if(strcmp(color, "gray") == 0){
		return 7;
	}
	return -1;
}

//verilen sayinin renk karsiligini parametreye yazar
void number2color(int num, char tmp[10]){
	
	if(num == 0){
		strcpy(tmp, "red");
	}	
	if(num == 1){
		strcpy(tmp, "green");
	}
	if(num == 2){
		strcpy(tmp, "blue");
	}
	if(num == 3){
		strcpy(tmp, "yellow");
	}
	if(num == 4){
		strcpy(tmp, "purple");
	}
	if(num == 5){
		strcpy(tmp, "black");
	}
	if(num == 6){
		strcpy(tmp, "white");
	}
	if(num == 7){
		strcpy(tmp, "gray");
	}
}

//gerekli kontroller yapilarak renk matrixi kullanicidan alinir
void matrix_from_user(int **matrix,int n){
	
	//chosen_num dizisi kullanicidan ilk satiri,
	//ayni renk ayni satirda ikinci kez olmayacak sekilde almamizi
	//saglayan kontrolu yaparken kullanilir.
	
	//Ayni zamanda diger satirlari alirken de hem ayni renk iki defa bulunmasin
	//hem de 1. satirdakinden farkli renk bulunmamasi kontrolu.
	
	int **chosen_num = (int**)calloc(2,sizeof(int*));
	chosen_num[0] = (int*)calloc(n,sizeof(int)); // 0. satirinda, matrisin ilk satirindaki sayilar
	chosen_num[1] = (int*)calloc(n,sizeof(int)); // 1. satirinda, bulunup bulunmadigina dair flagler tutar.
	
	char color_tmp[10]; //kullnicidan alinan renk stringi bu degiskende tutulur
	
	int i,j,k,tmp, ctrl = 0; //kontrol degiskeni flag olarak kullanilir.
	
	do{	
		printf("-------------------------------------------\n> Pick N different colors!\n");
		ctrl = 0;
		for(i = 0 ; i < n; i++){
			do{
				// ilk satiri ayni renkten bir kez olacak sekilde kullanicidan alir
				printf("0. row %d. col : ", i);
				scanf("%s",color_tmp);
				tmp = color2number(color_tmp);
				if(tmp == -1){ //renk yanlis yazilmissa veya listede olmayan bir renk girildi ise ayni index'i tekrar ister
					printf("> That is not in color list!\n");
				}
			}while(tmp == -1);
			
			chosen_num[0][i] = tmp;
			
			// Burada ctrl flag'i yardimi ile girilen renk onceden girilmis mi kontrolu yapilir
			for(j = i-1 ; j >= 0 && ctrl == 0; j--){
				if(chosen_num[0][j] == chosen_num[0][i]){
					ctrl = 1;
				}
			}
		}
	}while(ctrl == 1);
	
	//alinan ilk satiri matrix'in ilk satirina kopyalar
	for(i = 0 ; i < n ; i++){
		matrix[0][i] = chosen_num[0][i];
	}
	
	//burada diger satirlarda girilen renklerin ilk satirdakilerden farkli olmadigindan
	//veya ayni satira ayni rengin bir daha girilmediginden emin olunur. 
	i = 1;
	while(i < n){
		do{
			ctrl = 0;
			printf("-------------------------------------------\n> Do not pick different colors from the ones that in first row!\n> Also picked colors have to occure once!\n\n");
			for(j = 0 ; j < n ; j++){
				
				do{
					printf("%d. row %d. col : ", i,j);
					scanf("%s",color_tmp);
					tmp = color2number(color_tmp);
					if(tmp == -1){ // yukarida bahsedilen renk liste kontrolu
						printf("> That is not in color list!\n");
					}
				}while(tmp == -1);

				matrix[i][j] = tmp; 
				
				k = 0;
				while(k < n && chosen_num[0][k] != matrix[i][j]){
					k++;
				}
				if(chosen_num[0][k] == matrix[i][j]){
					if(chosen_num[1][k] == 0){
						chosen_num[1][k] = 1;
					}
					else{
						ctrl = 1; // o renk daha once girilmisse flag aktif edilir
					}
				}
				else{
					ctrl = 1; // o renk ilk satirda secilen renkler arasinda yoksa flag aktif edilir
				}
			}
			for(k = 0 ; k < n ; k++){
				chosen_num[1][k] = 0; // herhangi bir durumda satir kullanicidan yeniden isteneceginden
			}						  // flagler sifirlanir
			
		}while(ctrl == 1);
		i++;
	}
}

//matrixi renge donusturup ekrana yazar
void printMatrix(int **matrix, int n){
	int i, j;
	char color[10];
	for(i = 0 ; i < n ; i++){
		printf("\n");
		for(j = 0 ; j < n ; j++){
			number2color(matrix[i][j], color);
			printf("%10s",color);
		}
	}
	printf("\n----------------------------------\n");
}

//verilen satirin ustundeki satirlara bakarak istenilen durumda ise 1, cakisma varsa 0 doner
int is_matrix_okey_for_row(int **matrix, int n, int row){
	
	int ctrl = 1;
	int i,j;
	//(row-1)'den baslayip 0. satira kadar sutunlari karsilastirir
	for(i = row-1 ; i >= 0 ; i--){
		for(j = 0 ; j < n ; j++){
			if(matrix[i][j] == matrix[row][j]){
				ctrl = 0;
			}
		}
	}
	return ctrl;
}

//verilen satiri verilen sayi kadar saga kaydirir
void shift_given_row(int **matrix, int n, int row, int howMany){
	
	int i;
	int new_place; // tutulan indexin yeni yerini tutar
	int *tmp = (int*)calloc(n, sizeof(int)); // satiri kaydirilmis sekilde tmp bir diziye yazar
	
	for(i = 0 ;  i < n ; i++){
		new_place = (i + howMany)%n; // kaydirma miktari eski index'e eklenir ve dizi boyutuna gore modu alinir
		tmp[new_place] = matrix[row][i];  
	}
	//tmp dizi matrixdeki konumuna kaydirilmis olarak kopyalanir
	for(i = 0 ; i < n ; i++){
		matrix[row][i] = tmp[i];
	}
}

//backtracking algoritmasinin gerceklestirildigi recursive fonksiyon
//row --> uzerinde bulunulan row icin cagirilir fonksiyon
//whichPrint --> hangi sekilde yazdirmak istenildigi bilgisi
//shiftCounts --> her bir satirin kac kez kaydirildigi bu array'de tutulur
int backTrack(int **matrix, int n, int row, int whichPrint, int shiftCounts[10]){
	
	//gonderilen row son satira geldiyse artik cozum bulunmus demektir ve fonksiyon kendini cagirmayi birakir
	//gonderilen row cozum bulamadikca ust satira cikip, artik 0. satira geldiginde cozum yok demektir ve fonksiyon kendini cagirmayi birakir
	if(row < n && row > 0){
		
		// matrix'in cagirilan satiri ust satirlari ile istedigimiz kosullari sagliyorsa alt satira gecilir
		if(is_matrix_okey_for_row(matrix, n, row) == 1){
			if(whichPrint == 1){ 
				printf("Matrix is okey till %d th row: ",row);
				printMatrix(matrix, n); //Matrix (eger kullanici ara adimlari da yazdirmak istiyorsa) yazdirilir 
			}
			shiftCounts[row+1] = 0; // bir alt satirin shift sayisi 0'lanir cünkü bu alt satira farkli ust cozumlerden gelinmis olabilir
			backTrack(matrix, n, row+1, whichPrint, shiftCounts); // kosullar saglandigindan alt satir icin fonksiyon bir kez daha cagirilir
		}
		else{
			// satir ustundeki satirlarla istenilen duruma gelmediyse 1 kere saga kaydirilarak tekrar kontrol edilir
			shift_given_row(matrix,n,row, 1);
			shiftCounts[row]++;
			
			//satirimiz istenilen duruma gelene kadar kaydirma yapilir
			//fakat (n-1)'den fazla kaydirma yapmak mantiksiz olacagindan onun kontrolu yapilir
			if(shiftCounts[row] < n){
				backTrack(matrix,n,row, whichPrint, shiftCounts);  
			}
			else if(shiftCounts[row-1] < n){
				//(n-1) kere kaydirma islemi yapildiktan sonra hala istenilen duruma gelmiyorsa bir ust satira cikilir
				//cikilan bir ust satirda da kaydirma islemi yapilacagindan burada o satirin da (n-1)'den fazla shift yapmadigindan emin olunur
				printf("\n> Going back from %d th row to %d th row!\n\n----------------------------------\n", row, row-1);
				
				shift_given_row(matrix,n,row-1, 1);
				shiftCounts[row-1]++;
			
				backTrack(matrix,n,row-1, whichPrint, shiftCounts); //bir ust satir icin fonksiyon tekrar cagirilir
			}
		}
	}
	
	// butun kontroller ve islemler sonunda matris istenen duruma geldiyse 1 gelmediyse 0 dondurulur 
	if(is_matrix_okey_for_row(matrix, n, n-1) == 1){ // (n-1) ile bu fonksiyonu cagirmak aslinda butun matrisi kontrol etmek demek
		return 1;	
	}
	else{
		return 0;
	}
}

//Main fonksiyon
int main(){
	
	printf("~~~~~~~~Welcome Back, to The Future~~~~~~~~\n\n"); // havali giris cümlesi
	
	int n,i,j;
	
	// kullanici 3 ve 8 arasinda bir sayi girene kadar israr edilir
	do{
		printf("How many colors (N must be between 3 and 8): ");
		scanf("%d",&n);
	}while(n < 3 || n > 8);
	
	printf("\nColors:\n0> red\n1> green\n2> blue\n3> yellow\n4> purple\n5> black\n6> white\n7> gray\n");
	
	//matrix dinamik olarak olusturulur
	int **matrix = (int**)calloc(n, sizeof(int*));
	for(i = 0 ; i < n ; i++){
		matrix[i] = (int*)calloc(n, sizeof(int));
	}
	
	//matrix kullanicidan alinir
	matrix_from_user(matrix, n);
	printf("\n----------------------------------\n");
	printf("Given Matrix:");
	printMatrix(matrix, n);

	//islemler ne sekilde yazdirilacak secimi kullanicidan alinir
	int whichPrint;
	do{
		whichPrint = 0;
		printf("> 0 for just printing solution\n> 1 for printing also sub-processes \n");
		printf("Which way do you want to print the backtracking process: ");
		scanf("%d",&whichPrint);
		printf("\n----------------------------------\n");
	}while(whichPrint != 0 && whichPrint != 1);
	
	//her bir satira kac defa shift yapildigini tutacak olan degisken
	int shiftCounts[10] = {0,0,0,0,0,0,0,0,0,0};
	
	// Backtracking ile cozum olup olmadigini bulan fonksiyon cagirilir
	int ctrl = backTrack(matrix, n, 1, whichPrint, shiftCounts);
	// donen deger 1 ise cozum var demektir ve matrix bastirilir
	if(ctrl == 1){
		printf("Solution matrix is :");
		printMatrix(matrix,n);
	}
	else{
		// 0 donmus ise cozum yok demketir
		printf("No Solution!\n----------------------------------\n");
	}
	
	printf("\n~~~~~~~~developed by Sadi~~~~~~~~\n"); // havali cikis cumlesi
	return 0;
}
