﻿#ifndef __GMGLFRAMEBUFFER_H__
#define __GMGLFRAMEBUFFER_H__
#include <gmcommon.h>
BEGIN_NS

class GMGraphicEngine;
GM_PRIVATE_OBJECT(GMGLFramebuffer)
{
	const IRenderContext* context = nullptr;
	ITexture* texture = nullptr;
};

class GMGLFramebuffer : public GMObject, public IFramebuffer
{
	DECLARE_PRIVATE(GMGLFramebuffer);

public:
	GMGLFramebuffer(const IRenderContext* context);
	~GMGLFramebuffer();

public:
	virtual bool init(const GMFramebufferDesc& desc) override;
	virtual ITexture* getTexture() override;
	virtual void setName(const GMString& name) override {}
	virtual const IRenderContext* getContext();

public:
	GMuint getTextureId();
};

class GMGLDefaultFramebuffers;
GM_PRIVATE_OBJECT(GMGLFramebuffers)
{
	GMuint fbo = 0;
	GMuint depthStencilBuffer = 0;
	Vector<GMGLFramebuffer*> framebuffers;
	bool framebuffersCreated = false;
	GMGraphicEngine* engine = nullptr;
	GMFramebufferDesc desc;
	GMfloat clearColor[4];
	const IRenderContext* context = nullptr;
};

class GMGLFramebuffers : public GMObject, public IFramebuffers
{
	DECLARE_PRIVATE(GMGLFramebuffers)

public:
	GMGLFramebuffers(const IRenderContext* context);
	~GMGLFramebuffers();

	virtual bool init(const GMFramebuffersDesc& desc) override;
	virtual void addFramebuffer(AUTORELEASE IFramebuffer* framebuffer) override;
	virtual void use() override;
	virtual void bind() override;
	virtual void unbind() override;
	virtual void clear(GMFramebuffersClearType type) override;
	virtual IFramebuffer* getFramebuffer(GMsize_t) override;
	virtual GMsize_t count() override;
	virtual void copyDepthStencilFramebuffer(IFramebuffers* dest) override;

public:
	virtual const IRenderContext* getContext();

public:
	virtual GMuint framebufferId();

	const GMFramebufferDesc& getDesc()
	{
		D(d);
		return d->desc;
	}

private:
	void createFramebuffers();

protected:
	virtual void createDepthStencilBuffer(const GMFramebufferDesc& desc);
	virtual void setViewport();

public:
	static IFramebuffers* createDefaultFramebuffers(const IRenderContext* context);
};

GM_PRIVATE_OBJECT(GMGLShadowMapTexture)
{
	GMuint textureId;
};

class GMGLShadowMapTexture : public ITexture
{
	DECLARE_PRIVATE(GMGLShadowMapTexture)

public:
	GMGLShadowMapTexture(GMuint textureId);
	~GMGLShadowMapTexture();

	virtual void init() override;
	virtual void bindSampler(GMTextureSampler* sampler) override {}
	virtual void useTexture(GMint) override;
};

GM_PRIVATE_OBJECT(GMGLShadowFramebuffers)
{
	GMint width = 0;
	GMint height = 0;
	GMGLShadowMapTexture* shadowMapTexture = nullptr;
};

class GMGLShadowFramebuffers : public GMGLFramebuffers
{
	DECLARE_PRIVATE_AND_BASE(GMGLShadowFramebuffers, GMGLFramebuffers)

public:
	GMGLShadowFramebuffers(const IRenderContext* context);
	~GMGLShadowFramebuffers();

public:
	virtual bool init(const GMFramebuffersDesc& desc) override;
	virtual void use() override;
	virtual void bind() override;

public:
	inline GMint getShadowMapWidth()
	{
		D(d);
		return d->width;
	}

	inline GMint getShadowMapHeight()
	{
		D(d);
		return d->height;
	}

	inline ITexture* getShadowMapTexture()
	{
		D(d);
		return d->shadowMapTexture;
	}

private:
	virtual void createDepthStencilBuffer(const GMFramebufferDesc& desc);
};

END_NS
#endif