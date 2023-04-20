#ifndef _GILCOORD_
#define _GILCOORD_

typedef enum tagTCE_KTM_ORIGIN
{
	tmOriginWest	= 0,	// 38, 125
	tmOriginMiddle	= 1,	// 38, 127
	tmOriginEast	= 2,	// 38, 129
	tmOriginAuto	= -1,	//자동
} TCE_KTM_ORIGIN;




void		TcrTm2Bsl		(double *lat	, double *lon	, TCE_KTM_ORIGIN eOrigin = tmOriginMiddle);	// TM -> BESSEL
void		TcrBsl2Tm		(double *lat	, double *lon	, TCE_KTM_ORIGIN eOrigin = tmOriginMiddle);	// BESSEL -> TM

void		TcrWgs2Bsl		(double *lat	, double *lon	);	// WGS84	-> BESSEL






typedef enum tagTCE_COORD_ELLIPSOID
{
	TCE_COORD_ELLIPSOID_BESSEL1841	= 0,	//
	TCE_COORD_ELLIPSOID_WGS1984		= 1,	//
	TCE_COORD_ELLIPSOID_GRS80		= 2,	//
	TCE_NUM_COORD_ELLIPSOID
} TCE_COORD_ELLIPSOID;

typedef enum tagTCE_COORD_PARAM
{
	TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA	= 0,	//
	TCE_COORD_PARAM_TM_GRS80_SOUTH_KOREA		= 1,	//
	TCE_NUM_COORD_PARAM
} TCE_COORD_PARAM;

typedef enum tagTCE_TRANS_METHOD
{
	TCE_COORD_TRANS_MOLODENSKY					= 1,
	TCE_COORD_TRANS_BURSA						= 2,
} TCE_TRANS_METHOD;



typedef struct tagTCS_COORD_ELLIPSOID_DAT
{
	char	szName[64];
	double	a;	//장반경
	double	f;	//편평율
} TCS_COORD_ELLIPSOID_DAT;

typedef struct tagTCS_COORD_PARAM_DAT
{
	char					szName[64];
	TCE_COORD_ELLIPSOID		eEllipsoid;
	//평행이동량
	double					dx;			//단위 1m
	double					dy;			//단위 1m
	double					dz;			//단위 1m
	char					szArea[64];
	//회전량
	double					omega;		//단위 1초
	double					phi;		//단위 1초
	double					kappa;		//단위 1초
	//축척변화량
	double					ds;			//단위 1ppm
	//타원체 변환법
	TCE_TRANS_METHOD		method;
} TCS_COORD_PARAM_DAT;


void		TcrBsl2GrsKtm		(double* pLat, double* pLon, TCE_KTM_ORIGIN eOrigin = tmOriginMiddle);


//void		SetParam			(TCE_COORD_PARAM eParam, int nTMode);
//void		SetParam			(double dx, double dy, double dz, double omega, double phi, double kappa, double ds, int nTMode);

void		WGS84GPtoKTM		(double phiW, double lamW, double hW, TCE_KTM_ORIGIN eOrigin, double dLam, double& xb, double& yb, double& hB);
void		KTMtoWGS84GP		(double xb, double yb, double hB, TCE_KTM_ORIGIN eOrigin, double dLam, double& phiW, double& lamW, double& hW);

void		BSL41GPtoKTM		(double phi, double lam, TCE_KTM_ORIGIN eOrigin, double dLam, double& X, double& Y);
void		KTMtoBSL41GP		(double X, double Y, TCE_KTM_ORIGIN eOrigin, double dLam, double& phi, double& lam);

void		GRS80GPtoGRS80KTM	(double phi, double lam, TCE_KTM_ORIGIN eOrigin, double dLam, double& X, double& Y);


void		BSL41GPtoGRS80GP	(double phiB, double lamB, double hB, double& phiW, double& lamW, double& hW);


void		BSL41GPtoWGS84GP	(double phiB, double lamB, double hB, double& phiW, double& lamW, double& hW);
void		WGS84GPtoBSL41GP	(double phiW, double lamW, double hW, double& phiB, double& lamB, double& hB);

void		BSL41GPtoBSL41CTR__	(double phi, double lam, double h, double& X, double& Y, double& Z);
void		BCTR2BGP__			(double X, double Y, double Z, double& phi, double& lam, double& h);

void		BSL41CTRtoGRS80CTR	(double xb, double yb, double zB, double& XW, double& YW, double& zW);


void		BSL41CTRtoWGS84CTR	(double xb, double yb, double zB, double& XW, double& YW, double& zW);
void		WGS84CTRtoBSL41CTR	(double XW, double YW, double zW, double& xb, double& yb, double& zB);

void		WGS84CTRtoWGS84GP__	(double X, double Y, double Z, double& phi, double& lam, double& h);
void		WGS84GPtoWGS84CTR__	(double phi, double lam, double h, double& X, double& Y, double& Z);

void		GRS80CTRtoGRS80GP__	(double X, double Y, double Z, double& phi, double& lam, double& h);


void		GP2CTR__			(double phi, double lam, double h, double a, double f1, double& X, double& Y, double& Z);
void		CTR2GP__			(double X, double Y, double Z, double a, double f1, double& phi, double& lam, double& h);

void		TransBursa__		(TCS_COORD_PARAM_DAT* pTransParam, double XW, double YW, double zW, double& xb, double& yb, double& zB);
void		InverseBursa__		(TCS_COORD_PARAM_DAT* pTransParam, double xb, double yb, double zB, double& XW, double& YW, double& zW);

void		TransMolod__		(TCS_COORD_PARAM_DAT* pTransParam, double XW, double YW, double zW, double& xb, double& yb, double& zB);
void		InverseMolod__		(TCS_COORD_PARAM_DAT* pTransParam, double xb, double yb, double zB, double& XW, double& YW, double& zW);

void		GP2TM				(double phi, double lam, double a, double f1, double x0, double y0, double phi0, double lam0, double ok, double& xn, double& ye);
void		TM2GP				(double xn, double ye, double a, double f1, double x0, double y0, double phi0, double lam0, double ok, double& phi, double& lam);

void		check				(double& f);
double		fnSPHSN				(double a, double es, double sphi);
double		fnSPHTMD			(double ap, double bp, double cp, double dp, double ep, double sphi);
double		fnSPHSR				(double a, double es, double sphi);
double		fnDENOM				(double es, double sphi);

//#ifdef  __cplusplus
//}
//#endif

#endif
