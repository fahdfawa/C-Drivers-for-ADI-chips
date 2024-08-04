#ifndef MAX20360_FIELDS_H
#define MAX20360_FIELDS_H

//PMIC

typedef enum {
    DISABLED,
    ENABLED,
    MPC_CONTROL
}State;

typedef enum{
    LDO_1,
    LDO_2
}LDO_selection;

typedef enum{
	BUCK_1,
	BUCK_2,
	BUCK_3
}Buck_selection;

typedef enum{
	CHG,
	BUCK1,
	BUCK2,
	BUCK3,
	BOOST,
	BUCK_BOOST,
	LDO1,
	LDO2,
	LOCK_UNLOCK_ALL
}Lock_Selection;

#endif

#define BUCK1STEP_MAX20360_EVKIT   10
#define BUCK1STEP_MAX20360_HEVKIT  10
#define BUCK1STEP_MAX20360A        10
#define BUCK1STEP_MAX20360B        10
#define BUCK1STEP_MAX20360C        10
#define BUCK1STEP_MAX20360F        25
#define BUCK1STEP_MAX20360G        25
#define BUCK1STEP_MAX20360I        25
#define BUCK1STEP_MAX20360J        50
#define BUCK1STEP_MAX20360K        25
#define BUCK1STEP_MAX20360L        50
#define BUCK1STEP_MAX20360M        50
#define BUCK1STEP_MAX20360O        25
#define BUCK1STEP_MAX20360P        10

#define BUCK2STEP_MAX20360_EVKIT   25
#define BUCK2STEP_MAX20360_HEVKIT  25
#define BUCK2STEP_MAX20360A        10
#define BUCK2STEP_MAX20360B        10
#define BUCK2STEP_MAX20360C        10
#define BUCK2STEP_MAX20360F        25
#define BUCK2STEP_MAX20360G        25
#define BUCK2STEP_MAX20360I        50
#define BUCK2STEP_MAX20360J        50
#define BUCK2STEP_MAX20360K        50
#define BUCK2STEP_MAX20360L        50
#define BUCK2STEP_MAX20360M        50
#define BUCK2STEP_MAX20360O        50
#define BUCK2STEP_MAX20360P        50

#define BUCK3STEP                  50

#define BOOSTSTEP                  250

#define BBSTEP                     50







