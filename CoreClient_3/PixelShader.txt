//이전 쉐이더 결과를 받아서 입력값을 처리
//픽셀 쉐이더 1픽셀마다 호출됨
//SV_TARGET = 레지스터(저장소), vertex 와 다름


Texture2D		g_txColor : register(t0); //register(t0) - 텍스쳐 레지스터
Texture2D		g_txMask : register(t1);
SamplerState	g_Sample : register(s0);  //g_txColor 텍스쳐에서 픽셀을 가져와서 픽셀 셰이더에서 작업

//텍스쳐 좌표
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{	
	//텍스처에서 t좌표에 해당하는 컬러값(픽셀) 반환
	float4 color = g_txColor.Sample(g_Sample, input.t);
	float4 mask = g_txMask.Sample(g_Sample, input.t);
	float4 final = color;	
	// 소스알파(1) = 마스크이미지의 검정색부분은 불투명된다.나머지 투명
	// 소스알파(0) = 마스크이미지의 흰색부분은   투명된다.
	final.a = 1.0f-mask.r;
	final = final * input.c;
	return final;
}