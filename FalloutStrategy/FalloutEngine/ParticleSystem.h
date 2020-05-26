#include "j1Entity.h"
#include "vector"

class Emiter;

class ParticleSystem : public j1Entity
{
public:
	ParticleSystem();
	ParticleSystem(float x, float y);

	~ParticleSystem();

	void PushEmiter(Emiter& emiter);

	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void Desactivate();
	void Activate();

	bool IsActive();

	void Move(int x, int y);

	std::vector<Emiter> emiterVector;
private:

	std::vector<float>	position;

	bool active;
};

