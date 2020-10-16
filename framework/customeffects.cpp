#pragma once
#include "main.h"

colour_t c_rainbow() {
	static float rainbow_value = 0.0f;
	rainbow_value += .001;

	if (rainbow_value > 1.0f) {
		rainbow_value = 0.0f;
	}

	return colour_t::from_hsb(rainbow_value, 1.0f, 1.0f);
}

void chroma::initialize()
{
	CChromaSDKImpl chroma;
	chroma.Initialize();
}

RZEFFECTID inject_effect = GUID_NULL;
RZEFFECTID ingame_effect = GUID_NULL;

void chroma::kill()
{
	g_ChromaSDKImpl.DeleteEffectImpl(inject_effect);
	g_ChromaSDKImpl.DeleteEffectImpl(ingame_effect);
	CChromaSDKImpl::UnInitialize();
}

void chroma::on_inject()
{

	if (inject_effect == GUID_NULL)
	{
		RZEFFECTID Frame0 = GUID_NULL;
		RZEFFECTID Frame1 = GUID_NULL;
		RZEFFECTID Frame2 = GUID_NULL;
		RZEFFECTID Frame3 = GUID_NULL;
		RZEFFECTID Frame4 = GUID_NULL;
		RZEFFECTID Frame5 = GUID_NULL;
		RZEFFECTID Frame6 = GUID_NULL;
		RZEFFECTID Frame7 = GUID_NULL;

		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect = {};
		//ChromaSDK::Keyboard::v2::CUSTOM_EFFECT_TYPE Effect = {};

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_NONE, NULL, &Frame0);

		for (UINT r = 0; r<ChromaSDK::Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c<3; c++)
			{
				Effect.Color[r][c] = RGB(218, 23, 91);
			}
		}

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, &Frame1);

		for (UINT r = 0; r<ChromaSDK::Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c<6; c++)
			{
				Effect.Color[r][c] = RGB(218, 23, 91);
			}
		}

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, &Frame2);

		for (UINT r = 0; r<ChromaSDK::Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c<9; c++)
			{
				Effect.Color[r][c] = RGB(218, 23, 91);
			}
		}

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, &Frame3);

		for (UINT r = 0; r<ChromaSDK::Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c<12; c++)
			{
				Effect.Color[r][c] = RGB(218, 23, 91);
			}
		}

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, &Frame4);

		for (UINT r = 0; r<ChromaSDK::Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c<15; c++)
			{
				Effect.Color[r][c] = RGB(218, 23, 91);
			}
		}

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, &Frame5);

		for (UINT r = 0; r<ChromaSDK::Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c<18; c++)
			{
				Effect.Color[r][c] = RGB(218, 23, 91);
			}
		}

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, &Frame6);

		ChromaSDK::Keyboard::STATIC_EFFECT_TYPE StaticEffect = {};
		StaticEffect.Color = RGB(218, 23, 91);

		g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_STATIC, &StaticEffect, &Frame7);

		g_ChromaSDKImpl.CreateEffectGroup(&inject_effect);

		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame0, 200);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame1, 100);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame2, 100);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame3, 100);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame4, 100);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame5, 100);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame6, 100);
		g_ChromaSDKImpl.AddToGroup(inject_effect, Frame7, 100);
	}

	g_ChromaSDKImpl.SetEffectImpl(inject_effect);
}

void chroma::in_game()
{
	g_ChromaSDKImpl.DeleteEffectImpl(inject_effect); //overwrite the effect

	if (!vars::chroma::enabled || !vars::chroma::in_game) {
		return;
	}

	RZEFFECTID frame1 = GUID_NULL;
	RZEFFECTID frame2 = GUID_NULL;

	g_ChromaSDKImpl.CreateEffectGroup(&ingame_effect);

	ChromaSDK::Keyboard::CUSTOM_KEY_EFFECT_TYPE Effect = {};

	for (UINT r = 0; r < ChromaSDK::Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c < 22; c++)
		{
			Effect.Color[r][c] = RGB(41, 15, 43);
		}
	}

	Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_W)][LOBYTE(ChromaSDK::Keyboard::RZKEY_W)] = 0x01000000 | RGB(218, 23, 91);
	Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_A)][LOBYTE(ChromaSDK::Keyboard::RZKEY_A)] = 0x01000000 | RGB(218, 23, 91);
	Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_S)][LOBYTE(ChromaSDK::Keyboard::RZKEY_S)] = 0x01000000 | RGB(218, 23, 91);
	Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_D)][LOBYTE(ChromaSDK::Keyboard::RZKEY_D)] = 0x01000000 | RGB(218, 23, 91);

	Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_INSERT)][LOBYTE(ChromaSDK::Keyboard::RZKEY_INSERT)] = 0x01000000 | RGB(152, 16, 64);
	Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZLED_LOGO)][LOBYTE(ChromaSDK::Keyboard::RZLED_LOGO)] = 0x01000000 | RGB(c_rainbow().r(), c_rainbow().g(), c_rainbow().b());

	if (vars::visuals::custom_crosshair && vars::chroma::up_arrow) {
		Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_UP)][LOBYTE(ChromaSDK::Keyboard::RZKEY_UP)] = 0x01000000 | RGB(ctx::crosshair_colour.r(), ctx::crosshair_colour.g(), ctx::crosshair_colour.b());
	}

	if (vars::chroma::killstreak) {
		unsigned int killed = ctx::local_kills;
		unsigned int deaths = ctx::local_deaths;
		unsigned int key_index = 0;
		
		if (ctx::local_deaths > 0) {
			ctx::local_kills = 0;
			ctx::local_deaths = 0;
		}
		
		switch (killed) { //dumb switch statement...
			case 0: key_index = 1299; break;
			case 1: key_index = 1042; break;
			case 2: key_index = 1043; break;
			case 3: key_index = 1044; break;
			case 4: key_index = 786; break;
			case 5: key_index = 787; break;
			case 6: key_index = 788; break;
			case 7: key_index = 530; break;
			case 8: key_index = 531; break;
			case 9: key_index = 532; break;
		}

		if (killed <= 9) 
			Effect.Key[HIBYTE(key_index)][LOBYTE(key_index)] = 0x01000000 | RGB(c_rainbow().r(), c_rainbow().g(), c_rainbow().b());
		else {
			Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD9)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD9)] = 0x01000000 | RGB(c_rainbow().r(), c_rainbow().g(), c_rainbow().b());
			Effect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD_ADD)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD_ADD)] = 0x01000000 | RGB(c_rainbow().r(), c_rainbow().g(), c_rainbow().b());
		}

	}

	g_ChromaSDKImpl.CreateKeyboardEffectImpl(ChromaSDK::Keyboard::CHROMA_CUSTOM_KEY, &Effect, &frame2);

	g_ChromaSDKImpl.AddToGroup(ingame_effect, frame2, NULL);

	g_ChromaSDKImpl.SetEffectImpl(ingame_effect);
}
