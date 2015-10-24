#include "RenderEngine.h"

using namespace RenderEngine;

void RenderObjectEx::instantiate()
{
	referenceID = Renderer::instance().createObject(this);
}

void RenderObjectEx::update()
{
	Renderer::instance().updateObject(this, referenceID);
}

void RenderObjectEx::destroy()
{
	Renderer::instance().clearObject(referenceID);
}

unsigned int RenderObjectEx::getReferenceID()
{
	return referenceID;
}



unsigned int Renderer::newObject()
{
	//Search for an object in vector tagged Unused
	for (unsigned i = 0; i < objectList.size(); i++)
	{
		if (objectList[i].inUse == false)
		{
			return i;
		}
	}
	//if no unused objects, add a new one
	RenderObjectIn newObj = RenderObjectIn();
	newObj.inUse = true;
	objectList.push_back(newObj);
	return objectList.size() - 1;
}

void Renderer::init()
{
	objectList.reserve(100);
	gPass.init();
	lightPass.init();
	compositePass.init();
}

unsigned int Renderer::createObject(RenderObjectEx* in)
{
	unsigned int slot = newObject();
	objectList[slot] = *in;
	return slot;
}

void Renderer::updateObject(RenderObjectEx* in, unsigned int id)
{
	objectList[id] = *in;
}

void Renderer::clearObject(unsigned int id)
{
	objectList[id].inUse = false;
}


void Renderer::render(Window w)
{

}
