#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Input.h"
#include "Defs.h"
#include "Log.h"

#define VSYNC false

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

Render::~Render()
{
}

bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	vSync = VSYNC;

	return ret;
}

bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	SetVSync(vSync);

	return true;
}

bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

bool Render::Load(pugi::xml_node& load)
{
	camera.x = load.child("camera").attribute("x").as_int();
	camera.y = load.child("camera").attribute("y").as_int();

	LOG("X: %d", camera.x);

	return true;
}

bool Render::Save(pugi::xml_node& saveNode) const
{
	pugi::xml_node cam = saveNode.append_child("camera");

	cam.append_attribute("x").set_value(camera.x);
	cam.append_attribute("y").set_value(camera.y);

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetVSync(bool vSync)
{
	if (vSync)
	{
		SDL_GL_SetSwapInterval(-1);
	}
	else if (!vSync)
	{
		SDL_GL_SetSwapInterval(0);
	}
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, SDL_RendererFlip flip, bool useCamera) const
{
	bool ret = true;

	float scale = 1;

	SDL_Rect rect;
	if (useCamera)
	{
		rect.x = (int)(camera.x * speed) + x * scale;
		rect.y = (int)(camera.y * speed) + y * scale;
	}
	else if (!useCamera)
	{
		rect.x = x * scale;
		rect.y = y * scale;
	}

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if (useCamera)
	{
		rect.w *= scale;
		rect.h *= scale;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawTexture(SDL_Texture* texture, int x, int y, float scale, bool scaleModifiesCoordinates, const SDL_Rect* section, double angle, int pivotX, int pivotY, SDL_RendererFlip flip, bool useCamera) const
{
	bool ret = true;

	SDL_Rect rect;
	if (useCamera)
	{
		if (scaleModifiesCoordinates)
		{
			rect.x = (int)(camera.x) + x * scale;
			rect.y = (int)(camera.y) + y * scale;
		}
		else if (!scaleModifiesCoordinates)
		{
			rect.x = (int)(camera.x) + x;
			rect.y = (int)(camera.y) + y;
		}
	}
	else if (!useCamera)
	{
		if (scaleModifiesCoordinates)
		{
			rect.x = x * scale;
			rect.y = y * scale;
		}
		else if (!scaleModifiesCoordinates)
		{
			rect.x = x;
			rect.y = y;
		}
	}

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawTexture(SDL_Texture* texture, int x, int y, float scaleX, float scaleY, bool scaleModifiesCoordinates, const SDL_Rect* section, double angle, int pivotX, int pivotY, SDL_RendererFlip flip, bool useCamera) const
{
	bool ret = true;

	SDL_Rect rect;
	if (useCamera)
	{
		if (scaleModifiesCoordinates)
		{
			rect.x = (int)(camera.x) + x * scaleX;
			rect.y = (int)(camera.y) + y * scaleY;
		}
		else if (!scaleModifiesCoordinates)
		{
			rect.x = (int)(camera.x) + x;
			rect.y = (int)(camera.y) + y;
		}
	}
	else if (!useCamera)
	{
		if (scaleModifiesCoordinates)
		{
			rect.x = x * scaleX;
			rect.y = y * scaleY;
		}
		else if (!scaleModifiesCoordinates)
		{
			rect.x = x;
			rect.y = y;
		}
	}

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scaleX;
	rect.h *= scaleY;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawTexture(SDL_Texture* texture, int x, int y, float scale, const SDL_Rect* section, bool scaleModifiesCoordinates, double angle, SDL_RendererFlip flip) const
{
	bool ret = true;

	SDL_Rect rect;
	if (scaleModifiesCoordinates)
	{
		rect.x = (int)(camera.x) + x * scale;
		rect.y = (int)(camera.y) + y * scale;
	}
	else if (!scaleModifiesCoordinates)
	{
		rect.x = (int)(camera.x) + x;
		rect.y = (int)(camera.y) + y;
	}

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, SDL_Color color, bool filled, bool useCamera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_Rect rec(rect);
	if (useCamera)
	{
		rec.x = (int)(camera.x + rect.x);
		rec.y = (int)(camera.y + rect.y);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, SDL_Color color, bool useCamera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int result = -1;

	if (useCamera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, SDL_Color color, bool filled, bool useCamera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));

		if (useCamera)
		{
			points[i].x += camera.x;
			points[i].y += camera.y;
		}
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (filled)
	{
		for (double dy = 1; dy <= radius; dy += 1)
		{
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			if (useCamera)
			{
				SDL_RenderDrawLine(renderer, camera.x + x - dx, camera.y + y + dy - radius, camera.x + x + dx, camera.y + y + dy - radius);
				SDL_RenderDrawLine(renderer, camera.x + x - dx, camera.y + y - dy + radius, camera.x + x + dx, camera.y + y - dy + radius);
			}
			else
			{
				SDL_RenderDrawLine(renderer, x - dx, y + dy - radius, x + dx, y + dy - radius);
				SDL_RenderDrawLine(renderer, x - dx, y - dy + radius, x + dx, y - dy + radius);
			}
		}
	}

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}