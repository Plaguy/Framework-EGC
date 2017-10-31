/* --Animation2D.h-- */

class Animation2D
{
	public:
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;
		int status;
		float mintX, mintY, minr, maxtX, maxtY, maxr;

		Animation2D();
		Animation2D(float tX, float tY, float sX, float sY, float aStep, int s);
		~Animation2D();
		/*float GetTranslateX();
		float GetTranslateY();
		float GetScaleX();
		float GetScaleY();
		float GetAngularStep();
		int GetStatus();*/
};