#include <stdio.h>
const int GetData1()
{
	//1_���� ��ȯ�ϴ� �Լ�(�� ����=X)
	int iValue = 4;
	return iValue;
}
const int* GetData2()
{
	//2_���� �ּҸ� ��ȯ�ϴ� ������ �Լ�(�� ����=X / �ּҺ���=O)
	int iValue = 4;
	return &iValue;
}

const int* const  GetData3()
{
	//3_���� �ּҸ� ��ȯ�ϴ� ������ �Լ�(�� ����=X / �ּҺ���=X)
	int iValue = 4;
	return &iValue;
}
int* const  GetData4()
{
	//4_���� �ּҸ� ��ȯ�ϴ� ������ �Լ�(�� ����=O / �ּҺ���=X)
	int iValue = 4;
	return &iValue;
}
// mutable, frined, static_cast<> ����ȯ
void main(int argc, char* argv[])
{
	//-��ȯ�� ���� ����, ������ �ƴ�
	int iRet1 = GetData1();

	int iRet2 = 4;

	//-��ȯ�� �ּҸ� ����Ʈ ������ ����(pRet������ ����=X / �ּ� ����=O)
	//-GetData2()�� (�� ����=X / �ּҺ���=O) �ε� pRet������ �� ���� = X�ϱ� ����)
	const int* pRet = GetData2();
	//-pRet������ �� ������ �Ұ����ϴϱ� �Ұ���
	//*pRet = 88; // error
	
	//-GetData3()��(�� ����=X / �ּҺ���=X)�ε� �ּҰ��� ������ ���� ���������ϱ� �Ұ���
	//int iRet3 = GetData3();
	//-GetData3()��ȯ�� ���� X�ε� iRet3�� �ʱⰪ���� �ް� �� ���� ���Ѵٱ� ������ ����
	const int* iRet3 = GetData3();
	//-iRet3�� �� ���� �Ұ���!
	//*iRet3 = 9; // error
	//-iRet3�� �ּҰ��� ���� ���������ϱ� ���氡��
	iRet3 = &iRet2;

	//-GetData3()�� (�� ����=X / �ּҺ���=X)�ε� ����� ������ �Ѵ� ���� ���Ѵٱ� ������ ����
	const int* const iRet4 = GetData3();
	//-iRet4 �Ѵ� ���� �Ұ��� �̴ϱ� �Ұ���
	//*iRet4 = 9; // error
	//iRet4 = &iRet2;// error

	//-GetData4()�� (�� ����=O / �ּҺ���=X)�̰� ������ �������� ���� ���Ѵٱ� �ϴϱ� ����
	const int* iRet5 = GetData4();
	//- �� ���� �Ұ����̴ϱ� �Ұ���
	//*iRet5 = 9; // error
	//-iRet5������ �ּҴ� ���� ���� �ϴ����ϱ� ����
	iRet5 = &iRet2;

	//
	int* const iRet6 = GetData4();
	//-iRet6�� �� ���� ����
	*iRet6 = 9;
	//-�ּҴ� ���� �Ұ���
	//iRet6 = &iRet2;// error

	int data1 = 1;
	int data2 = 4;
	const int* dataConst1 = &data1;
	const int const* dataConst3 = &data1;
	int* const dataConst4 = &data1;
	//// ������ 
	//*dataConst1 = 4; // error
	//*dataConst3 = 4; // error
	*dataConst4 = 4;
	// �ּ�
	dataConst1 = &data2;
	int* const dataConst2 = &data1;
	*dataConst2 = 7;
	//dataConst2 = &data2;// error

	const int* const dataConst9 = &data1;
	//*dataConst9 = 7; // error
	//dataConst9 = &data2; // error
}
