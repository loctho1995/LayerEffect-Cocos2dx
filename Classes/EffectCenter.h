#pragma once
#include "cocos2d.h"

using namespace cocos2d;

struct EffectData
{
public:
	enum DataTypes 
	{
		Blur,
		Bloom,
		BrightFilter
	};

	char* programName; //ten cua shader program
	DataTypes effectType;
	RenderTexture *renderTexture; //render len RenderTexture
	Sprite *originalSprite; //luu Texture
	bool isClearRenderTexture = true; //co clear renderTexture khong
};

struct BlurData : public EffectData
{
public:
	BlurData()
	{
		this->effectType = EffectData::Blur;
	}

	enum BlurTypes
	{
		VerticalBlur,
		HorizontalBlur,
		Kernal9Blur,
		GaussianBlur,
		GaussianBlur8Directions
	};
	
	BlurTypes type; //loai blur
	float radius; //ban kinh blur
	cocos2d::Vec2 direction; //huong blur
};

struct BrightFilterData : public EffectData
{
	BrightFilterData()
	{
		this->effectType = EffectData::BrightFilter;
	}

	float threshold; // = 0 thi khong co phan tang loc do sang
};

struct BloomData : public EffectData
{
	BloomData()
	{
		this->effectType = EffectData::Bloom;
	}

	float brightThreshold = 0.0f; //nguong loc anh sang
	float radius; //ban binh bloom
	float constract = 1.0f; //do tuong phan (cang lon thi anh cang sang)
};

class EffectCenter : public Node
{
public:
	EffectCenter();
	~EffectCenter();

    //blur by combine horizontal blur kernal 11 and vertical blur kernal 11
    static void blur(RenderTexture *texture, int radius);

    //blur by horizontal direction with kernal 11
	static void horizontalBlur(RenderTexture *texture, int radius);

    //blur by vertical direction with kernal 11
	static void VerticalBlur(RenderTexture *texture, int radius);

    //blur kernal 9 with 8 pixel around main pixel
	static void blurKernel9(RenderTexture *texture, int radius);

    //blur by gaussian function - normal pixelSize = 1 / width and 1 / height
	static void gaussianBlur(RenderTexture *texture, Size pixelSize, Vec2 direction, int radius);

    //blur by gaussian blur with 8 direction around main pixel
	static void gaussianBlur8Directions(RenderTexture *texture, Size pixelSize, int radius);
	static void gaussianBlur8Directions(RenderTexture *texture, int radius);
    /*
    bright filter with threshold
    filter by threshold which have luma conversion higher than threshold will be show
    (texColor.r * 0.2126) + (texColor.g * 0.7152) + (texColor.b * 0.0722)

    if threshold = 0 (by default) it will show all by multiply pixel color with luma conversion
    */
	static void brightFilter(RenderTexture *texture, float threshold = 0);

    /*
        bloom texture by radius and birght contrast
        radius is radius of blur 
        contrast higher the texture more brihgter
        brightThreshold nguong loc anh sang dung cho BrightFilter
    */
	static void bloom(RenderTexture *texture, float radius = 5.0f, float brightThreshold = 0.0f, float contrast = 1.0f);

	static void distortion(RenderTexture *texture, float weightX, float weightY);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void onDraw(EffectData *data);

protected:

    //calculate gaussian function
    static void calculateGaussianWeights(const int points, float* weights);

    static void initShaders();

    static bool isInitShader;

	std::vector<EffectData*> effectDatas;

	std::vector<CustomCommand*> commands;
};

