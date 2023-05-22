#include "RenderThread.h"
#include "ConcurrentQueue.h"
#include <Renderer.h>

std::thread* RenderThread::t = nullptr;
Queue<Command> RenderThread::q;
volatile bool RenderThread::alive = false;
volatile int RenderThread::framesDiff = 0;


void RenderThread::Init()
{
	if (t != nullptr) return;

	alive = true;
	t = new std::thread(ProcessQueue);
}

void RenderThread::End()
{
	if (t == nullptr) return;

	alive = false;
	//present o nuevo comando "endFrame/Close"
	Enqueue({ CommandType::present });
	t->join();
	delete(t);
	t = nullptr;
}

void RenderThread::Enqueue(Command command_)
{
	if (t == nullptr) return;
	if (command_.type == CommandType::present) framesDiff++;
	q.push(command_);
}

void RenderThread::ProcessQueue()
{
	while (alive) {
		Command c = q.pop();

		switch (c.type)
		{
		case CommandType::clear:
		{
			Renderer::Instance()->clear(c.clear_dt.color);
			break;
		}
		case CommandType::drawRect:
		{
			DrawRect_dt drawt = c.drawRect_dt;
			Renderer::Instance()->drawRect(drawt.x, drawt.y, drawt.width, drawt.height, drawt.color);
			break;
		}
		case CommandType::present:
		{
			Renderer::Instance()->present();
			framesDiff--;
			break;
		}
		case CommandType::putPixel:
		{
			PutPixel_dt pixeldt = c.putPixel_dt;
			Renderer::Instance()->putPixel(pixeldt.x, pixeldt.y, pixeldt.color);
			break;
		}
		case CommandType::drawImage:
		{
			DrawImage_dt drawImagedt = c.drawImage_dt;
			Renderer::Instance()->drawImage(drawImagedt.source, drawImagedt.sx1, drawImagedt.sy1, drawImagedt.sx2, drawImagedt.sy2,
				drawImagedt.tx1, drawImagedt.ty1, drawImagedt.tx2, drawImagedt.ty2);
			break;
		}
		case CommandType::drawSprite:
		{
			DrawSprite_dt drawSpritedt = c.drawSprite_dt;
			drawSpritedt.s.draw(drawSpritedt.tx1, drawSpritedt.ty1, drawSpritedt.w, drawSpritedt.h);
			break;
		}
		default:
			/*here...they will never find you.*/
			break;
		}
	}
}