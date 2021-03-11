/*
 * smart_fridge.c
 *
 *  Created on: 22 lis 2020
 *      Author: andre
 */
#include "smart_fridge.h"
#define BUFFER_SIZE (64)


#if INITIAL_PRODUCTS == 0
Product products[MAX_NUMBER_OF_PRODUCTS];
uint32_t products_numb = 0;
#else


Product products[MAX_NUMBER_OF_PRODUCTS]={
		{4,"Mlec","Milk","Mlekovita","3.2 percent","1l","25.02.2021","00"},
		{4,"Mlec","Milk","Mlekovita","3.2 percent","1l","26.02.2021","00"},
		{4,"Mlec","Milk","Mlekovita","3.2 percent","1l","23.02.2021","00"},
		{4,"Mlec","Milk","Mleczna dolina","3.2 percent","1l","25.02.2021","00"},
		{4,"Mleg","Cheese","Swiatowid","","500 g","28.02.2021","00"},
		{4,"Mleh","Eggs","Ale Jaja","From free range","12 pieces","05.03.2021","00"},

		{4,"Mlea","Beer","Warka","6 percent","0.5l","13.08.2021","20"},
		{4,"Mleb","Beer","Zatecky","5.5 percent","0.5l","13.02.2021","20"},

		{4,"Mlem","Vodka","Sobieski","Pure 40 percent","1l","13.02.2022","20"},



		{4,"Mlej","Pizza","Feliciana","Frozen","2 x 325 g","13.07.2021","70"},

		{4,"Mlek","Tripe","Szubryt","Beef","485 g","23.05.2021","60"},
		{4,"Mles","Ham","Krakus","Large slices","120 g","19.04.2021","60"},
		{4,"Mleu","Sausages","Berlinki","With cheese","250 g","13.07.2021","60"},

		{4,"Mlen","Drink","Pepsi","","330 ml","25.07.2022","10"},

		{4,"Mlep","Mustard","Kamis","Sarepska","210 g","20.04.2021","30"},





		{4,"Mlet","Tuna","Marinero","Pieces in vegetable oil","185 g","03.04.2021","50"},

		{4,"Mlew","Orange juice","Riviva","","1l","13.03.2021","10"},
		{4,"Mkek","Orange juice","Riviva","","1l","13.03.2021","10"}};
uint32_t products_numb = 18;
#endif




// Buffers with grouped data for send to ThingSpeak
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



volatile bool thingSpeak_UpdateAvailable = false;
volatile bool flashUpdateAvailable = false;
volatile bool shopListChanged = false;
volatile bool thingSpeak_shopListChanged = false;

volatile int deletePosition;


char receiveBuffer[4000] = {0};
char requestBuffer[2000] = {0};

// Group for send to ThingSpeak
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

	//Add names of products to one list
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
	k = 0;


	//Add brands of products to one list
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
	k = 0;

	//Add products details to one list
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
		k = 0;


		//Add capacities of products to one list
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
		k = 0;


	//Add expiriation dates to one list
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
	k = 0;


	//Add products categories to one list
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

	k=0;

	//Add products from shopping list to one list
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


}


// Send all products and shop list to ThingSpeak
void SF_sendProductsToThingSpeak(char (*shoplist)[SHOPLIST_NAME_SIZE]){
	memset(receiveBuffer, 0, 4000*sizeof(char));
	memset(requestBuffer,0,2000*sizeof(char));

	groupProductData(shoplist);

	sprintf(requestBuffer,"/channels/1242116/feeds.json?api_key=%s",S1_DELETE_API);
	httpDelete(WWW_SERVER,requestBuffer,receiveBuffer);
	memset(receiveBuffer, 0, 4000*sizeof(char));
	memset(requestBuffer,0,2000*sizeof(char));

	sprintf(requestBuffer,"/update?api_key=%s&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s&field7=%s",S1_UPDATE_API,groupedNames,groupedBrands,groupedDetails,groupedCapacity,groupedDates,groupedCategorys,groupedShopList);
	httpPost(WWW_SERVER,requestBuffer,receiveBuffer);


	thingSpeak_UpdateAvailable = false;
	thingSpeak_shopListChanged = false;
}


void fullProductsListSignal(void){
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(1000));
	GPIO_PinWrite(GPIO,1,22,0);
}

// Signal for adding product to list
void addProductSignal(void){
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

//Signal for removing product from list
void removeProductSignal(void){
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(75));
	GPIO_PinWrite(GPIO,1,22,0);
	vTaskDelay(MSEC_TO_TICK(100));
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(200));
	GPIO_PinWrite(GPIO,1,22,0);
}


void networkConnectedSignal(void){
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(200));
	GPIO_PinWrite(GPIO,1,22,0);
	vTaskDelay(MSEC_TO_TICK(200));
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(200));
	GPIO_PinWrite(GPIO,1,22,0);
	vTaskDelay(MSEC_TO_TICK(200));
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(400));
	GPIO_PinWrite(GPIO,1,22,0);
}

void networkDisconnectSignal(void){
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(300));
	GPIO_PinWrite(GPIO,1,22,0);
	vTaskDelay(MSEC_TO_TICK(300));
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(900));
	GPIO_PinWrite(GPIO,1,22,0);
}

void wifiModuleInitSignal(void){
	GPIO_PinWrite(GPIO,1,22,1);
	vTaskDelay(MSEC_TO_TICK(500));
	GPIO_PinWrite(GPIO,1,22,0);
}


// Function called every 500 ms, to check if card is in range and if it is - read data from it
bool SF_detectProduct(void){
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
		return false;
	}
	RC522_Halt();
	RC522_StopCrypto1();
	thingSpeak_UpdateAvailable = true;
	flashUpdateAvailable = true;
	return true;

}

// Call at program start, configure RC522 reader
void SF_startRFID_Module(SPI_Type *base,int rstPort,int rstPin){
    RC522_Init(base,rstPort,rstPin);
    RC522_GetFirmwareVersion();
}

// Sort products list after delete product
void sortList(void){
	for(int i=deletePosition;i<products_numb-1;i++){
		products[i] = products[i+1];
	}
	products_numb--;
}

// Reading from specific pages in which we write data before
void NTAG215_read_config(void){
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
// Reading from specific blocks in which we write data before
void MIFARE_read_config(void){
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


// Read data from card
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
	// Check if product is on list, comparing UID
	for(int i=0;i<=products_numb;i++){
		n = 0;
		for(int j=0;j<productUid.size;j++){
			if(productUid.uidByte[j] == products[i].uidByte[j]){
				n++;
			}
		}
		//If all bytes in compared match - this product is on list and we delete it
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

			// Buzzer signal for remove product from list
			removeProductSignal();
		}
	}
	// If variable isOnList is still false, product isn't on list - we read data from card an add it
	if(!isOnList){
		if(products_numb < MAX_NUMBER_OF_PRODUCTS){
			//Reading data from blocks --------------------------------------------------------------------
			len = 18;
			if(rfid_card == NTAG215)
				RC522_NTAG215_AUTH(PSWBuff,pACK);

			// ----------------------------------- READ PRODUCT NAME ----------------------------
			if(rfid_card == MIFARE){
				status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)THIRD_SECTOR_TRAILER, &key, &productUid); //line 834
				if (status != STATUS_OK) {
					return false;
				}
			}
			status = RC522_MIFARE_Read(PRODUCT_NAME_BLOCK1, buffer, &len);
			if (status != STATUS_OK) {
				return false;
			}


			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productName[k] = '\0';
					}

					else{
						products[products_numb].productName[k] = buffer[k];
					}

				}

			}

			if(readMore){
				status = RC522_MIFARE_Read(PRODUCT_NAME_BLOCK2, buffer, &len);
				if (status != STATUS_OK) {
					PRINTF("Reading failed: ");
					return false;
				}

				for(int k=0;k<16;k++){
					if(readMore){
						if(buffer[k] == '#'){
							readMore = false;
							products[products_numb].productName[16+k] = '\0';
						}
						else{
							products[products_numb].productName[16+k] = buffer[k];
						}

					}

				}

			}

			// --------------------------------- READ BRAND -------------------------------------
			readMore = true;
			if(rfid_card == MIFARE){
				status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FOURTH_SECTOR_TRAILER, &key, &productUid); //line 834
				if (status != STATUS_OK) {
					return false;
				}
			}
			status = RC522_MIFARE_Read(PRODUCT_BRAND_BLOCK1, buffer, &len);
			if (status != STATUS_OK) {
				return false;
			}


			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productBrand[k] = '\0';
					}

					else{
						products[products_numb].productBrand[k] = buffer[k];
					}
				}
			}

			if(readMore){
				status = RC522_MIFARE_Read(PRODUCT_BRAND_BLOCK2, buffer, &len);
				if (status != STATUS_OK) {
					PRINTF("Reading failed: ");
					return false;
				}

				for(int k=0;k<16;k++){
					if(readMore){
						if(buffer[k] == '#'){
							readMore = false;
							products[products_numb].productBrand[16+k] = '\0';
						}
						else{
							products[products_numb].productBrand[16+k] = buffer[k];
						}

					}

				}

			}
			// ---------------------- READ DETAILS -------------------------------------
			readMore = true;
			if(rfid_card == MIFARE){
				status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FIFTH_SECTOR_TRAILER, &key, &productUid); //line 834
				if (status != STATUS_OK) {
					return false;
				}
			}
			status = RC522_MIFARE_Read(PRODUCT_DETAILS_BLOCK1, buffer, &len);
			if (status != STATUS_OK) {
				return false;
			}


			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productDetails[k] = '\0';
					}

					else{
						products[products_numb].productDetails[k] = buffer[k];

					}
				}
			}

			if(readMore){
				status = RC522_MIFARE_Read(PRODUCT_DETAILS_BLOCK2, buffer, &len);
				if (status != STATUS_OK) {
					PRINTF("Reading failed: ");
					return false;
				}

				for(int k=0;k<16;k++){
					if(readMore){
						if(buffer[k] == '#'){
							readMore = false;
							products[products_numb].productDetails[16+k] = '\0';
						}
						else{
							products[products_numb].productDetails[16+k] = buffer[k];
						}

					}

				}

			}

			// ---------------------- READ CAPACITY -------------------------------------
			if(rfid_card == MIFARE){
				status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)THIRD_SECTOR_TRAILER, &key, &productUid); //line 834
				if (status != STATUS_OK) {
					return false;
				}
			}
			readMore = true;
			status = RC522_MIFARE_Read(PRODUCT_CAPACITY_BLOCK, buffer, &len);
			if (status != STATUS_OK) {
				return false;
			}


			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productCapacity[k] = '\0';
					}

					else{
						products[products_numb].productCapacity[k] = buffer[k];
					}
				}
			}


			// ---------------------- READ EXPIRATION DATE -------------------------------------
			if(rfid_card == MIFARE){
				status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FOURTH_SECTOR_TRAILER, &key, &productUid); //line 834
				if (status != STATUS_OK) {
					return false;
				}
			}
			readMore = true;
			status = RC522_MIFARE_Read(PRODUCT_EXPIRATION_DATE_BLOCK, buffer, &len);
			if (status != STATUS_OK) {
				return false;
			}


			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].expirationDate[k] = '\0';
					}

					else{
						products[products_numb].expirationDate[k] = buffer[k];
					}
				}
			}

			// ---------------------- READ CATEGORY DATE -------------------------------------
			if(rfid_card == MIFARE){
				status = RC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A, (uint8_t)FIFTH_SECTOR_TRAILER, &key, &productUid); //line 834
				if (status != STATUS_OK) {
					return false;
				}
			}
			readMore = true;
			status = RC522_MIFARE_Read(PRODUCT_CATEGORY_BLOCK, buffer, &len);
			if (status != STATUS_OK) {
				return false;
			}


			for(int k=0;k<16;k++){
				if(readMore){
					if(buffer[k] == '#'){
						readMore = false;
						products[products_numb].productCategory[k] = '\0';
					}

					else{
						products[products_numb].productCategory[k] = buffer[k];
					}
				}
			}

			for(int i=0;i<productUid.size;i++){
				products[products_numb].uidByte[i] = productUid.uidByte[i];
			}

			//Add UID size
			products[products_numb].size = productUid.size;

			products_numb++;

			// Buzzer signal for add product
			addProductSignal();
		}else{
			fullProductsListSignal();
		}
	}

	return true;

}
