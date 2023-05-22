#pragma once
#ifndef RENDERTHREAD_H_
#define RENDERTHREAD_H_

#include <stdint.h>
#include <thread>
#include <Image.h>
#include <Sprite.h>
/*COMMANDS RELATED CODE*/

//Types of commands that can be sent to the Rendering engine.
enum CommandType { clear, putPixel, drawRect, drawImage, drawSprite, present };

//Packages of data carried by commands.
struct Clear_dt { uint32_t color; };
struct PutPixel_dt { float x; float y; uint32_t color; };
struct DrawRect_dt { float x; float y; float width; float height; uint32_t color; };
struct DrawImage_dt { const Image* source; float sx1; float sy1; float sx2; float sy2; float tx1; float ty1; float tx2; float ty2; };
struct DrawSprite_dt { Sprite s; float tx1; float ty1; float w; float h; };
//constructoras según el tipo de comando que se desee usar
struct Command
{
	CommandType type;

	Command(uint32_t color, CommandType type_ = CommandType::clear) : type(type_)
	{
		clear_dt = { color };
	}
	Command(uint32_t color, float x, float y, CommandType type_ = CommandType::putPixel) : type(type_)
	{
		putPixel_dt = { x,y, color };
	}
	Command(uint32_t color, float x, float y, float width, float height, CommandType type_ = CommandType::drawRect) : type(type_)
	{
		drawRect_dt = { x,y,width,height, color };
	}
	Command(const Image* source, float sx1, float sy1, float sx2, float sy2, float tx1, float ty1, float tx2, float ty2, CommandType type_ = CommandType::drawImage) : type(type_)
	{
		drawImage_dt = { source, sx1, sy1, sx2, sy2, tx1,ty1,tx2,ty2 };

	}
	Command(Sprite s, float tx1, float ty1, float w, float h, CommandType type_ = CommandType::drawSprite) : type(type_)
	{
		drawSprite_dt = { s,tx1,ty1,w,h };
	}
	Command(CommandType type_ = CommandType::present) : type(type_) {}

	union
	{
		Clear_dt clear_dt;
		PutPixel_dt putPixel_dt;
		DrawRect_dt drawRect_dt;
		DrawImage_dt drawImage_dt;
		DrawSprite_dt drawSprite_dt;
	};
};

/*RENDERTHREAD CLASS*/
template <typename T>
class Queue;
class RenderThread
{
public:
	//Starts the thread.
	static void Init();
	//Ends the thread and removes the data structures.
	static void End();
	//Adds a new command to the "Concurrent Queue".
	static void Enqueue(Command command_);
	static int getFramesDiff() { return framesDiff; }
private:
	static std::thread* t;
	static Queue<Command> q;

	static volatile bool alive;
	static volatile int framesDiff;

	//While the thread is alive, pops the next element in the queue and calls the renderer function corresponding to the popped command.
	static void ProcessQueue();
};

#endif