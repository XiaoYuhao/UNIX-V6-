#include "CharDevice.h"
#include "Utility.h"
#include "Kernel.h"

/*==============================class CharDevice===============================*/
CharDevice::CharDevice()
{
	this->m_TTy[0] = NULL;
	this->m_TTy[1] = NULL;
}

CharDevice::~CharDevice()
{
	//nothing to do here
}

void CharDevice::Open(short dev, int mode)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Open()!");
}

void CharDevice::Close(short dev, int mode)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Close()!");
}

void CharDevice::Read(short dev)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Read()!");
}

void CharDevice::Write(short dev)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Write()!");
}

void CharDevice::SgTTy(short dev, TTy* pTTy)
{
	Utility::Panic("ERROR! Base Class: CharDevice::SgTTy()!");
}



/*==============================class ConsoleDevice===============================*/
/* 
 * 这里定义派生类ConsoleDevice的对象实例。
 * 该实例对象中override了字符设备基类中
 * Open(), Close(), Read(), Write()等虚函数。
 */
ConsoleDevice g_ConsoleDevice;
extern TTy g_TTy1;
extern TTy g_TTy2;
ConsoleDevice::ConsoleDevice()
{
	//nothing to do here

}

ConsoleDevice::~ConsoleDevice()
{
	//nothing to do here
}

void ConsoleDevice::Open(short dev, int mode)
{
	short minor = Utility::GetMinor(dev);/*获取从设备号*/
	User& u = Kernel::Instance().GetUser();

	if ( minor != 0 && minor !=1 )	/* 选择的不是console */
	{
		return;
	}
	if ( NULL == this->m_TTy[0] )
	{
		this->m_TTy[0] = &g_TTy1;
		this->m_TTy[1] = &g_TTy2;
	}

	if(	minor==0 ){
		u.u_procp->p_ttyp = this->m_TTy[0];	// 将进程与tty终端设备绑定
	}
	else if( minor==1 ){
		u.u_procp->p_ttyp = this->m_TTy[1];
	}
/*
	if ( (TTy*)0 == u.u_procp->p_ttyp )
	{
		u.u_procp->p_ttyp = this->m_TTy[0];
	}
	if ( (TTy*)1 == u.u_procp->p_ttyp)
	{
		u.u_procp->p_ttyp = this->m_TTy[1];
	}
*/
	/* 设置设备初始模式 */
	if ( (this->m_TTy[1]->t_state & TTy::ISOPEN) == 0 )
	{
		this->m_TTy[1]->t_state = TTy::ISOPEN | TTy::CARR_ON;
		this->m_TTy[1]->t_flags = TTy::ECHO;
		this->m_TTy[1]->t_erase = TTy::CERASE;
		this->m_TTy[1]->t_kill = TTy::CKILL;
		this->m_TTy[1]->ntty=1;
	}

	/* 设置设备初始模式 */
	if ( (this->m_TTy[0]->t_state & TTy::ISOPEN) == 0 )
	{
		this->m_TTy[0]->t_state = TTy::ISOPEN | TTy::CARR_ON;
		this->m_TTy[0]->t_flags = TTy::ECHO;
		this->m_TTy[0]->t_erase = TTy::CERASE;
		this->m_TTy[0]->t_kill = TTy::CKILL;
		this->m_TTy[0]->ntty=0;
	}
}

void ConsoleDevice::Close(short dev, int mode)
{
	//nothing to do here
}

void ConsoleDevice::Read(short dev)
{
	short minor = Utility::GetMinor(dev);
	User& u = Kernel::Instance().GetUser();

	if ( 0 == minor || 1 == minor)
	{									/* 根据调用这个函数的进程绑定的tty进行读写*/
		u.u_procp->p_ttyp->TTRead();	/* 判断是否选择了console */
	}
}

void ConsoleDevice::Write(short dev)
{
	short minor = Utility::GetMinor(dev);
	User& u = Kernel::Instance().GetUser();
	if ( 0 == minor || 1 == minor)
	{									/* 根据调用这个函数的进程绑定的tty进行读写*/
		u.u_procp->p_ttyp->TTWrite();	/* 判断是否选择了console */
	}
}

void ConsoleDevice::SgTTy(short dev, TTy *pTTy)
{
}
