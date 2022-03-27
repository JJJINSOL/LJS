#include "PlaneObj.h"
bool PlaneObj::SetVertexData()
{
	// Local Coord
	// Box
	//  0        1 
	//     ¿øÁ¡
	//  2        3
	m_VertexList.resize(4);
	// -Z plane
	m_VertexList[0].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[0].n = Vector3(0.0f, 0.0f, -1.0f);
	m_VertexList[0].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[0].t = Vector2(0.0f, 0.0f);

	m_VertexList[1].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[1].n = Vector3(0.0f, 0.0f, -1.0f);
	m_VertexList[1].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[1].t = Vector2(1.0f, 0.0f);


	m_VertexList[2].p = Vector3(-1.0f, -1.0f, 0.0f);
	m_VertexList[2].n = Vector3(0.0f, 0.0f, -1.0f);
	m_VertexList[2].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[2].t = Vector2(0.0f, 1.0f);


	m_VertexList[3].p = Vector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].n = Vector3(0.0f, 0.0f, -1.0f);
	m_VertexList[3].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[3].t = Vector2(1.0f, 1.0f);
	return true;
}
bool PlaneObj::SetIndexData()
{
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}
PlaneObj::PlaneObj()
{

}
PlaneObj::~PlaneObj()
{

}