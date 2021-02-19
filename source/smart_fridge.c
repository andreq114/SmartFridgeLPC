/*
 * smart_fridge.c
 *
 *  Created on: 22 lis 2020
 *      Author: andre
 */
#include "smart_fridge.h"
#define BUFFER_SIZE (64)


#if INITIAL_PRODUCTS == 0
Product products[25];
uint32_t products_numb = 0;
#else
Product products[25]={{4,"Mlec","Mleko","Mleczna dolina","Wysoki procent","1l","13.08.2020","00"},
					  {4,"Mlec","Piwko","Warka","Wysoki procent","1l","13.08.2021","20"},
					  {4,"Mlec","Serek","Almette","Duzo bialka","200g","13.08.2025","00"},
					  {4,"Mlec","Mleko","Mleczna dolina","Wysoki procent","1l","13.01.2020","20"},
					  {4,"Mlec","Mleko","Mleczna dolina","Wysoki procent","1l","13.02.2020","20"},
					  {4,"Mlec","Mleko","Mleczna dolina","Wysoki procent","1l","12.08.2020","20"},
					  {4,"Mlec","Mleko","Mleczna dolina","Wysoki procent","1l","15.02.2021","20"},
					  {4,"Mlec","Mleko","Mleczna dolina","Wysoki procent","1l","13.02.2021","20"}};
uint32_t products_numb = 8;
#endif





char groupedNames[255];
char groupedBrands[255];
char groupedDetails[255];
char groupedCapacity[255];
char groupedDates[255];
char groupedCategorys[100];
char groupedShopList[255];

enum TARGET rfid_card;
int PRODUCT_NAME_BLOCK1;			//Block 12 for MIFARE 1K Classic, Pages 8-11 for NTAG215
int PRODUCT_NAME_BLOCK2;			//Block 13 for MIFARE 1K Classic, Pages 12-15 for NTAG215
int PRODUCT_BRAND_BLOCK1;			//Block 16 for MIFARE 1K Classic, Pages 16-19 for NTAG215
int PRODUCT_BRAND_BLOCK2;			//Block 17 for MIFARE 1K Classic, Pages 20-23 for NTAG215
int PRODUCT_DETAILS_BLOCK1;			//Block 20 for MIFARE 1K Classic, Pages 28-31 for NTAG215
int PRODUCT_DETAILS_BLOCK2;			//Block 21 for MIFARE 1K Classic, Pages 32-35 for NTAG215
int PRODUCT_CAPACITY_BLOCK;			//Block 14 for MIFARE 1K Classic, Pages 24-27 for NTAG215
int PRODUCT_EXPIRATION_DATE_BLOCK;	//Block 18 for MIFARE 1K Classic, Pages 36-39 for NTAG215
int PRODUCT_CATEGORY_BLOCK;			//Block 22 for MIFARE 1K Classic, Pages 40-43 for NTAG215



bool ring = false;

bool detected = false;

volatile bool dataUpdateAvailable = false;
volatile bool shopListChanged = false;

volatile int deletePosition;

lv_obj_t *labelek;

char bufferek[4000] = {0};
char request[2000] = {0};

void groupProductData(char (*shoplist)[SHOPLIST_NAME_SIZE]){
	memset(groupedNames, 0, 255*sizeof(char));
	memset(groupedBrands, 0, 255*sizeof(char));
	memset(groupedDetails, 0, 255*sizeof(char));
	memset(groupedCapacity, 0, 255*sizeof(char));
	memset(groupedDates, 0, 255*sizeof(char));
	memset(groupedCategorys, 0, 100*sizeof(char));
	memset(groupedShopList, 0, 255*sizeof(char));
	int i = 0;
	int j = 0;
	int k = 0;

	//Dodajemy nazwy produktow do jednej listy
	for(i=0;i<products_numb;i++){
		j = 0;
		while(products[i].productName[j] != '\0'){
			groupedNames[k] = products[i].productName[j];
			j++;
			k++;
		}
		groupedNames[k] = '$';
		k++;
	}
	//groupedNames[k] = '\0';
	k = 0;


	//Dodajemy marki produktow do jednej listy
	for(i=0;i<products_numb;i++){
		j = 0;
		while(products[i].productBrand[j] != '\0'){
			groupedBrands[k] = products[i].productBrand[j];
			j++;
			k++;
		}
		groupedBrands[k] = '$';
		k++;
	}
	//groupedUid[k] = '\0';
	k = 0;

	//Dodajemy szczegoly produktow do jednej listy
		for(i=0;i<products_numb;i++){
			j = 0;
			while(products[i].productDetails[j] != '\0'){
				groupedDetails[k] = products[i].productDetails[j];
				j++;
				k++;
			}
			groupedDetails[k] = '$';
			k++;
		}
		//groupedNames[k] = '\0';
		k = 0;


		//Dodajemy pojemnosci/wagi produktow do jednej listy
		for(i=0;i<products_numb;i++){
			j = 0;
			while(products[i].productCapacity[j] != '\0'){
				groupedCapacity[k] = products[i].productCapacity[j];
				j++;
				k++;
			}
			groupedCapacity[k] = '$';
			k++;
		}
		//groupedUid[k] = '\0';
		k = 0;


	//Dodajemy daty produktow do jednej listy
	for(i=0;i<products_numb;i++){
		j = 0;
		while(products[i].expirationDate[j] != '\0'){
			groupedDates[k] = products[i].expirationDate[j];
			j++;
			k++;
		}
		groupedDates[k] = '$';
		k++;
	}
	//groupedDates[k] = '\0';
	k = 0;


	//Dodajemy kategorie produktow do jednej listy
	for(i=0;i<products_numb;i++){
		j = 0;
		while(products[i].productCategory[j] != '\0'){
			groupedCategorys[k] = products[i].productCategory[j];
			j++;
			k++;
		}
		groupedCategorys[k] = '$';
		k++;
	}
	//groupedCategorys[k] = '\0';

	k=0;
	for(int i=0; shoplist[i][0] != '\0' ; i++)
	{
		for(int j = 0; shoplist[i][j] != '\0'; j++){
			groupedShopList[k]= shoplist[i][j];
			k++;
		}
		groupedShopList[k] = '$';
		k++;
	}
	k=0;




	/*char tempe[255];
	sprintf(tempe,"%s",groupedNames);
	dispPRINTF(tempe,labelek);

	dispPRINTF("\n",labelek);
	sprintf(tempe,"%s",groupedUid);
	dispPRINTF(tempe,labelek);

	dispPRINTF("\n",labelek);
	sprintf(tempe,"%s",groupedDates);
	dispPRINTF(tempe,labelek);

	dispPRINTF("\n",labelek);
	sprintf(tempe,"%s",groupedCategorys);
	dispPRINTF(tempe,labelek);*/
}


void setConsoleLabel(lv_obj_t *label){
	labelek = label;
}

void dispPRINTF(const char *text,lv_obj_t *label){

		lv_textarea_add_text(label, text);

}


void sendProductsToThinkspeak(char (*shoplist)[SHOPLIST_NAME_SIZE]){
	memset(bufferek, 0, 4000*sizeof(char));
	memset(request,0,2000*sizeof(char));

	groupProductData(shoplist);


	sprintf(request,"/update?api_key=4M4LTMQWRYUS085E&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s&field7=%s",groupedNames,groupedBrands,groupedDetails,groupedCapacity,groupedDates,groupedCategorys,groupedShopList);

	httpDelete(WWW_SERVER,"/channels/1242116/feeds.json?api_key=XCILHJ0VPN5BY1X7",bufferek);
	memset(bufferek, 0, 4000*sizeof(char));

	httpPost(WWW_SERVER,request,bufferek);


	dataUpdateAvailable = false;
	shopListChanged = false;
}






bool detectProduct(){

	if(!RC522_IsNewCardPresent()){
		return false;
	}
	if(!RC522_ReadCardSerial())
		return false;
	Uid current_product_uid = RC522_getUid();

	if(current_product_uid.size == 4){
		MIFARE_read_config();
	}
	else{
		NTAG215_read_config();
	}

	if(!readProductsData(current_product_uid)){
		//dispPRINTFsec("Cos poszlo nie tak\n");
	}



	//WYPISANIE W TERMINALU INFORMACJI O OSTATNIO DODANYM PRODUKCIE-----------------------------------------------

	//Po czytaniu z sektorów
	RC522_Halt();
	RC522_StopCrypto1();
	dataUpdateAvailable = true;
	//tileview_Refresh();
	//const uint8_t *name = products[0].expirationDate;
	//dispPRINTF(name,labelek);
	/*for (uint8_t i = 0; i < products[index-1].size; i++) {
		dispPRINTF((char)products[index-1].uidByte[i],labelek);
	}*/
	//dispPRINTF("\n",labelek);
	//j = 0;

	//const char *temp = products[0].productCategory;
	//uint8_t *temp = &products[0].productCategory;

	//lv_textarea_set_text(labelek, "");
	//char tempe[48];
	//for(int h=0;h<index;h++){
		//sprintf(tempe,"%s",products[h].productName);
		//dispPRINTF(tempe,labelek);
		//dispPRINTF(",",labelek);

		//sprintf(tempe,"%s",products[h].expirationDate);
		//dispPRINTF(tempe,labelek);
		//dispPRINTF(",",labelek);

		//sprintf(tempe,"%s",products[h].productCategory);
		//dispPRINTF(tempe,labelek);
		//dispPRINTF("\n",labelek);


	//}
	//sprintf(tempe,"%d",index);
	//dispPRINTF("Liczba produktow: ",labelek);
	//dispPRINTF(tempe,labelek);
	//sendProductsToThinkspeak();
	//j++;
	/*
	dispPRINTF("\n",labelek);
	j = 0;
	while(products[index-1].productName[j] != '#'){

		dispPRINTF((char*)products[index-1].productName[j],labelek);
		j++;
	}
	dispPRINTF("\n",labelek);
	dispPRINTF((char*)products[index-1].productCategory,labelek);*/
	//dispPRINTF((char*)products[0].productName,labelek);
	return true;

}

void startRFID_Module(SPI_Type *base,int rstPort,int rstPin){
    RC522_Init(base,rstPort,rstPin);
    RC522_GetFirmwareVersion();
}

void sortList(){
	for(int i=deletePosition;i<products_numb-1;i++){
		products[i] = products[i+1];
	}
	products_numb--;
}


void NTAG215_read_config(){
	rfid_card = NTAG215;
	PRODUCT_NAME_BLOCK1 = 8;
	PRODUCT_NAME_BLOCK2 = 12;
	PRODUCT_BRAND_BLOCK1 = 16;
	PRODUCT_BRAND_BLOCK2 = 20;
	PRODUCT_DETAILS_BLOCK1 = 28;
	PRODUCT_DETAILS_BLOCK2 = 32;
	PRODUCT_CAPACITY_BLOCK = 24;
	PRODUCT_EXPIRATION_DATE_BLOCK = 36;
	PRODUCT_CATEGORY_BLOCK = 40;
}

void MIFARE_read_config(){
	rfid_card = MIFARE;
	PRODUCT_NAME_BLOCK1 = 12;
	PRODUCT_NAME_BLOCK2 = 13;
	PRODUCT_BRAND_BLOCK1 = 16;
	PRODUCT_BRAND_BLOCK2 = 17;
	PRODUCT_DETAILS_BLOCK1 = 20;
	PRODUCT_DETAILS_BLOCK2 = 21;
	PRODUCT_CAPACITY_BLOCK = 14;
	PRODUCT_EXPIRATION_DATE_BLOCK = 18;
	PRODUCT_CATEGORY_BLOCK = 22;
}


bool readProductsData(Uid productUid){
	enum StatusCode status;

	uint8_t buffer[18];
	bool readMore = true;
	uint8_t len;
	MIFARE_KEY key;
	int n;
	// NTAG KEY AND ACK
	uint8_t PSWBuff[] = {0xFF, 0xFF, 0xFF, 0xFF}; // 32 bit password default FFFFFFFF.
	uint8_t pACK[] = {0, 0}; // 16 bit password ACK returned by the NFCtag.

	for (uint8_t i = 0; i < 6; i++){
		key.keyByte[i] = 0xFF;
	}
	bool isOnList = false;
	// Sprawdzamy czy produkt jest na liscie porównując UID
	for(int i=0;i<=products_numb;i++){
		n = 0;
		for(int j=0;j<productUid.size;j++){
			if(productUid.uidByte[j] == products[i].uidByte[j]){
				n++;
			}
		}
		//Jeśli wszystkie bajty w porównywanych się zgadzają,produkt jest już na liście i go usuwamy
		if(n == productUid.size){

			deletePosition = i;
			for(int k = 0; k<products[i].size;k++)
				products[i].uidByte[k] = 0;

			products[i].size = 0;

			for(int k=0;k<33;k++)
				products[i].productName[k] = 0;

			for(int k=0;k<33;k++)
				products[i].productBrand[k] = 0;

			for(int k=0;k<33;k++)
				products[i].productDetails[k] = 0;

			for(int k=0;k<17;k++)
				products[i].productCapacity[k] = 0;

			for(int k=0;k<11;k++)
				products[i].expirationDate[k] = 0;

			products[i].productCategory[0] = 0;

			sortList();

			isOnList = true;
			GPIO_PinWrite(GPIO,1,22,1);
			vTaskDelay(MSEC_TO_TICK(75));
			GPIO_PinWrite(GPIO,1,22,0);
			vTaskDelay(MSEC_TO_TICK(100));
			GPIO_PinWrite(GPIO,1,22,1);
			vTaskDelay(MSEC_TO_TICK(200));
			GPIO_PinWrite(GPIO,1,22,0);
		}
	}

	if(!isOnList){
		//dispPRINTF("DODAJE\n",labelek);
		//Reading data from blocks --------------------------------------------------------------------
		//int byte_num = 0;
		len = 18;
		if(rfid_card == NTAG215)
			RC522_NTAG215_AUTH(PSWBuff,pACK);

		// ----------------------------------- READ PRODUCT NAME ----------------------------
		if(rfid_card == MIFARE){
			status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)THIRD_SECTOR_TRAILER, &key, &productUid); //line 834
			if (status != STATUS_OK) {
				//dispPRINTFsec("Authentication failed!\n");
				return false;
			}
		}
		status = RC522_MIFARE_Read(PRODUCT_NAME_BLOCK1, buffer, &len);
		if (status != STATUS_OK) {
			//dispPRINTFsec("Reading failed: ");
			return false;
		}


		for(int k=0;k<16;k++){
			if(readMore){
				if(buffer[k] == '#'){
					readMore = false;
					products[products_numb].productName[k] = '\0';
					//byte_num++;
				}

				else{
					products[products_numb].productName[k] = buffer[k];
					//byte_num++;
				}

			}

		}

		if(readMore){
			status = RC522_MIFARE_Read(PRODUCT_NAME_BLOCK2, buffer, &len);
			if (status != STATUS_OK) {
				PRINTF("Reading failed: ");
				//checkStatus(status);
				return false;
			}

			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productName[16+k] = '\0';
						//byte_num++;
					}
					else{
						products[products_numb].productName[16+k] = buffer[k];
						//byte_num++;
					}

				}

			}

		}

		// --------------------------------- READ BRAND -------------------------------------
		readMore = true;
		if(rfid_card == MIFARE){
			status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FOURTH_SECTOR_TRAILER, &key, &productUid); //line 834
			if (status != STATUS_OK) {
				//dispPRINTFsec("Authentication failed!\n");
				return false;
			}
		}
		status = RC522_MIFARE_Read(PRODUCT_BRAND_BLOCK1, buffer, &len);
		if (status != STATUS_OK) {
			//dispPRINTFsec("Reading failed: ");

			return false;
		}


		for(int k=0;k<16;k++){
			if(readMore){
				if(buffer[k] == '#'){
					readMore = false;
					products[products_numb].productBrand[k] = '\0';
					//byte_num++;
				}

				else{
					products[products_numb].productBrand[k] = buffer[k];
					//byte_num++;

				}
			}
		}

		if(readMore){
			status = RC522_MIFARE_Read(PRODUCT_BRAND_BLOCK2, buffer, &len);
			if (status != STATUS_OK) {
				PRINTF("Reading failed: ");
				//checkStatus(status);
				return false;
			}

			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productBrand[16+k] = '\0';
						//byte_num++;
					}
					else{
						products[products_numb].productBrand[16+k] = buffer[k];
						//byte_num++;
					}

				}

			}

		}
		// ---------------------- READ DETAILS -------------------------------------
		readMore = true;
		if(rfid_card == MIFARE){
			status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FIFTH_SECTOR_TRAILER, &key, &productUid); //line 834
			if (status != STATUS_OK) {
				//dispPRINTFsec("Authentication failed!\n");
				return false;
			}
		}
		status = RC522_MIFARE_Read(PRODUCT_DETAILS_BLOCK1, buffer, &len);
		if (status != STATUS_OK) {
			//dispPRINTFsec("Reading failed: ");

			return false;
		}


		for(int k=0;k<16;k++){
			if(readMore){
				if(buffer[k] == '#'){
					readMore = false;
					products[products_numb].productDetails[k] = '\0';
					//byte_num++;
				}

				else{
					products[products_numb].productDetails[k] = buffer[k];
					//byte_num++;

				}
			}
		}

		if(readMore){
			status = RC522_MIFARE_Read(PRODUCT_DETAILS_BLOCK2, buffer, &len);
			if (status != STATUS_OK) {
				PRINTF("Reading failed: ");
				//checkStatus(status);
				return false;
			}

			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productDetails[16+k] = '\0';
						//byte_num++;
					}
					else{
						products[products_numb].productDetails[16+k] = buffer[k];
						//byte_num++;
					}

				}

			}

		}

		// ---------------------- READ CAPACITY -------------------------------------
		if(rfid_card == MIFARE){
			status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)THIRD_SECTOR_TRAILER, &key, &productUid); //line 834
			if (status != STATUS_OK) {
				//dispPRINTFsec("Authentication failed!\n");
				return false;
			}
		}
		readMore = true;
		status = RC522_MIFARE_Read(PRODUCT_CAPACITY_BLOCK, buffer, &len);
		if (status != STATUS_OK) {
			//dispPRINTFsec("Reading failed: ");

			return false;
		}


		for(int k=0;k<16;k++){
			if(readMore){
				if(buffer[k] == '#'){
					readMore = false;
					products[products_numb].productCapacity[k] = '\0';
					//byte_num++;
				}

				else{
					products[products_numb].productCapacity[k] = buffer[k];
					//byte_num++;
					//data[index].name[k] = buffer[k];
				}
			}
		}


		// ---------------------- READ EXPIRATION DATE -------------------------------------
		if(rfid_card == MIFARE){
			status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FOURTH_SECTOR_TRAILER, &key, &productUid); //line 834
			if (status != STATUS_OK) {
				//dispPRINTFsec("Authentication failed!\n");
				return false;
			}
		}
		readMore = true;
		status = RC522_MIFARE_Read(PRODUCT_EXPIRATION_DATE_BLOCK, buffer, &len);
		if (status != STATUS_OK) {
			//dispPRINTFsec("Reading failed: ");

			return false;
		}


		for(int k=0;k<16;k++){
			if(readMore){
				if(buffer[k] == '#'){
					readMore = false;
					products[products_numb].expirationDate[k] = '\0';
					//byte_num++;
				}

				else{
					products[products_numb].expirationDate[k] = buffer[k];
					//byte_num++;
					//data[index].name[k] = buffer[k];
				}
			}
		}

		// ---------------------- READ CATEGORY DATE -------------------------------------
		if(rfid_card == MIFARE){
			status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FIFTH_SECTOR_TRAILER, &key, &productUid); //line 834
			if (status != STATUS_OK) {
			//	dispPRINTFsec("Authentication failed!\n");
				return false;
			}
		}
		readMore = true;
		status = RC522_MIFARE_Read(PRODUCT_CATEGORY_BLOCK, buffer, &len);
		if (status != STATUS_OK) {
			//dispPRINTFsec("Reading failed: ");

			return false;
		}


		for(int k=0;k<16;k++){
			if(readMore){
				if(buffer[k] == '#'){
					readMore = false;
					products[products_numb].productCategory[k] = '\0';
					//byte_num++;
				}

				else{
					products[products_numb].productCategory[k] = buffer[k];
					//byte_num++;
					//data[index].name[k] = buffer[k];
				}
			}
		}

		for(int i=0;i<productUid.size;i++){
			products[products_numb].uidByte[i] = productUid.uidByte[i];
		}
				//Dodajemy dlugosc UID produktu
		products[products_numb].size = productUid.size;
				//Dodajemy nazwe produktu

				//Dodajemy date waznosci produktu

				//Dodajemy kategorie produktu
		products_numb++;
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(50));
		GPIO_PinWrite(GPIO,1,22,0);
		vTaskDelay(MSEC_TO_TICK(50));
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(50));
		GPIO_PinWrite(GPIO,1,22,0);
		vTaskDelay(MSEC_TO_TICK(50));
		GPIO_PinWrite(GPIO,1,22,1);
		vTaskDelay(MSEC_TO_TICK(50));
		GPIO_PinWrite(GPIO,1,22,0);

	}

	return true;

}
