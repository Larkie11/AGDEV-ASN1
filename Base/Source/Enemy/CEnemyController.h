#include "Enemy.h"
#include "Vector3.h"

enum BEHAVIORS
{
	GOTOWP,
	IDLE,
};
class CEnemyController : public CEnemy
{
public:
	CEnemyController();
	virtual ~CEnemyController();
	void Update(double dt);
	void Init();
private:
	CEnemy* theEnemy;
	Vector3 moveto;
	bool arrived = false;
};