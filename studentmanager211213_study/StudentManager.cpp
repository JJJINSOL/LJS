#include "StudentManager.h"

NodeBox* const StudentManager::NewNode() 
{
	NodeBox* user = new NodeBox(userCounter);
	return user;
}
void StudentManager::AddLink(NodeBox* const pUser) 
{
	if (headUser == nullptr)
	{
		headUser = pUser;
		endUser = pUser;
		userCounter++;
	}
	else 
	{
		endUser->m_next = pUser;
		endUser = pUser;
		userCounter++;
	}

}
void StudentManager::Create()
{
	for (int i = 0; i < 1; i++)
	{
		AddLink(NewNode());
	}
}
bool StudentManager:: FileSave(const char* pFileName)
{
	FILE* fp = fopen(pFileName, "wb");
	int count = userCounter;
	fwrite(&count, sizeof(int), 1, fp);
	for (NodeBox* user = headUser; user != nullptr; user = user->m_next)
	{
		user->m_Data->Save();
		fwrite(&user->m_Data->m_type, sizeof(int), 1, fp);
		fwrite(&user->m_Data->m_buffersize, sizeof(int), 1, fp);
		fwrite(user->m_Data->m_buffer, user->m_Data->m_buffersize, 1, fp);
	}
	fclose(fp);
	return true;
}
void StudentManager::allDel()
{
	NodeBox* unext = headUser;
	while(unext)
	{
		NodeBox* del = unext;
		unext = del->m_next;
		delete del;
		del = nullptr;
		userCounter--;
	}
	headUser = nullptr;
}
void StudentManager::Load(const char* pFileName)
{
	FILE* fp = fopen(pFileName, "rb");
	int count = 0;
	fread(&count, sizeof(int), 1, fp);
	if (fp != nullptr)
	{
		for (int i = 0; i < count; i++)
		{
			NodeBox* user = new NodeBox();
			memset(user, 0, sizeof(NodeBox));
			int type = -1;
			fread(&type, sizeof(int), 1, fp);
			if (type == 1)
				user->m_Data = new StudentMiddle();
			else if (type == 2)
				user->m_Data = new StudentHigh();
			else 
				user->m_Data = new StudentCollege();

			fread(&user->m_Data->m_buffersize, sizeof(int), 1, fp);
			fread(&user->m_Data->m_buffer, user->m_Data->m_buffersize, 1, fp);

			user->m_Data->Load();
			AddLink(user);
		}
		fclose(fp);
		fp = nullptr;
	}
}
void StudentManager::Draw()
{}
std::ostream& operator << (std::ostream& os, StudentManager& mgr)
{
	for (NodeBox* user = mgr.headUser; user != nullptr; user = user->m_next)
	{
		user->m_Data->Show();
	}
	return os;
}