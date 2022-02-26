#pragma once
#include "NetUser.h"
class ChatUser : public NetUser, public ObjectPool<ChatUser>
{
};

