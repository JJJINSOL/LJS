cbuffer cb0 : register(b0)
{
	// 1개의 레지스터(x,y,z,w)
	matrix   g_matWorld : packoffset(c0);
	matrix   g_matView : packoffset(c4);
	matrix   g_matProj : packoffset(c8);
	matrix	 g_matNormal : packoffset(c12);
	float4   Color0 : packoffset(c16);	
	float    TimerX : packoffset(c17.x); // Timer.x, Timer.y, Timer.z, Timer.w	
};
struct VS_OUTPUT
{
    float4 p  : SV_POSITION; 
	float4 c  : COLOR0;
};
//--------------------------------------------------------------------------------------
// Composite partices into the scene
// 텍스쳐/depth 맵 출력 -> 원래 투영좌표에 대한 zw값을 x,y 에 대입
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 p : POSITION )
{
    VS_OUTPUT output;
	output.p = mul(p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);

	output.c = (output.p.w - Color0.z) / (Color0.w - Color0.z);
    return output;
}

float4 PS(VS_OUTPUT vIn) : SV_Target
{
	//랜더타겟에 직접 depth 값 저장

	float color = (vIn.p.w - Color0.z) / (Color0.w - Color0.z);
	return float4(color, color, color, 1);
}