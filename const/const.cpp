#include <stdio.h>
const int GetData1()
{
	//1_값을 반환하는 함수(값 변경=X)
	int iValue = 4;
	return iValue;
}
const int* GetData2()
{
	//2_값의 주소를 반환하는 포인터 함수(값 변경=X / 주소변경=O)
	int iValue = 4;
	return &iValue;
}

const int* const  GetData3()
{
	//3_값의 주소를 반환하는 포인터 함수(값 변경=X / 주소변경=X)
	int iValue = 4;
	return &iValue;
}
int* const  GetData4()
{
	//4_값의 주소를 반환하는 포인터 함수(값 변경=O / 주소변경=X)
	int iValue = 4;
	return &iValue;
}
// mutable, frined, static_cast<> 형변환
void main(int argc, char* argv[])
{
	//-반환값 저장 가능, 변경이 아님
	int iRet1 = GetData1();

	int iRet2 = 4;

	//-반환된 주소를 포인트 변수에 넣음(pRet변수값 변경=X / 주소 변경=O)
	//-GetData2()는 (값 변경=X / 주소변경=O) 인데 pRet변수도 값 변경 = X니까 가능)
	const int* pRet = GetData2();
	//-pRet변수는 값 변경이 불가능하니까 불가능
	//*pRet = 88; // error
	
	//-GetData3()는(값 변경=X / 주소변경=X)인데 주소값을 넣으면 변경 가능해지니까 불가능
	//int iRet3 = GetData3();
	//-GetData3()반환값 변경 X인데 iRet3가 초기값으로 받고 값 변경 안한다구 했으니 가능
	const int* iRet3 = GetData3();
	//-iRet3은 값 변경 불가능!
	//*iRet3 = 9; // error
	//-iRet3는 주소값은 변경 가능했으니까 변경가능
	iRet3 = &iRet2;

	//-GetData3()는 (값 변경=X / 주소변경=X)인데 선언된 변수도 둘다 변경 안한다구 했으니 가능
	const int* const iRet4 = GetData3();
	//-iRet4 둘다 변경 불가능 이니까 불가능
	//*iRet4 = 9; // error
	//iRet4 = &iRet2;// error

	//-GetData4()는 (값 변경=O / 주소변경=X)이고 변수가 값받으면 변경 안한다구 하니까 가능
	const int* iRet5 = GetData4();
	//- 값 변경 불가능이니까 불가능
	//*iRet5 = 9; // error
	//-iRet5변수는 주소는 변경 가능 하댔으니까 가능
	iRet5 = &iRet2;

	//
	int* const iRet6 = GetData4();
	//-iRet6는 값 변경 가능
	*iRet6 = 9;
	//-주소는 변경 불가능
	//iRet6 = &iRet2;// error

	int data1 = 1;
	int data2 = 4;
	const int* dataConst1 = &data1;
	const int const* dataConst3 = &data1;
	int* const dataConst4 = &data1;
	//// 데이터 
	//*dataConst1 = 4; // error
	//*dataConst3 = 4; // error
	*dataConst4 = 4;
	// 주소
	dataConst1 = &data2;
	int* const dataConst2 = &data1;
	*dataConst2 = 7;
	//dataConst2 = &data2;// error

	const int* const dataConst9 = &data1;
	//*dataConst9 = 7; // error
	//dataConst9 = &data2; // error
}
