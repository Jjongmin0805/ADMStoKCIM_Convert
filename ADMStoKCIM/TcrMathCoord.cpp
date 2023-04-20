#include "StdAfx.h"
#include "TcrMathCoord.h"
#include <math.h>


/////////////////////////////////////////////////////////////////////////////////////////
//	TcrCoord.cpp
/////////////////////////////////////////////////////////////////////////////////////////


#define PI	3.14159265358979323846

#define GLMF_rad(x)		((x) * PI/ 180)

#define GLMC_a			(6377397.155)				/* tokyo datum */
#define GLMC_ee			(0.006674372231315)

#define GLMC_false_e	(400000)	// KATECH
#define GLMC_false_n	(600000)
#define GLMC_false_x	(500000)	// TM
#define GLMC_false_y	(200000)

#define GLMC_en_m0		(0.9999)
#define GLMC_pow2en_m0	(0.99980001)
#define GLMC_pow3en_m0	(0.999700029999)
#define GLMC_pow4en_m0	(0.9996000599960001)
#define GLMC_pow5en_m0	(0.99950009999000049999)
#define GLMC_m0			(1.)
#define GLMC_origin_lon	(128)

#define GLMC_A			(1.005037306048555)
#define GLMC_B			(0.005047849240300)
#define GLMC_C			(0.000010563786831)
#define GLMC_D			(0.000000020633322)


/////////////////////////////////////////////////////////////////////////////////////////
//	prototype
double	fnxco(double olat);



/////////////////////////////////////////////////////////////////////////////////////////
//	implementation

double fnxco(double olat)
{
	double fnxco;

	fnxco=GLMC_a*(1-GLMC_ee)*((GLMC_A*olat)-0.5*GLMC_B*sinl(2*olat)+0.25*GLMC_C*sin(4*olat)
			-1./6.*GLMC_D*sin(6*olat));
	return fnxco;
}

void TcrTm2Bsl(double *lat, double *lon, TCE_KTM_ORIGIN eOrigin)	// TM -> BESSEL
{
	double n0,phi1, BN, nm, error, origin_lat, nn, M, N, t;
//	float Z;
//	int i, X, Y;
	double lon1, lat1;
	double xy_lon0;

	xy_lon0	= 0.0;

	lat1 = *lat - GLMC_false_x;
	lon1 = *lon - GLMC_false_y;

	switch ( eOrigin )
	{
	case tmOriginWest :
		xy_lon0=125.+10.405/3600.;
		break;
	case tmOriginMiddle :
		xy_lon0=127.+10.405/3600.;
		break;
	case tmOriginEast :
		xy_lon0=129.+10.405/3600.;
		break;
	}

	origin_lat = GLMF_rad(38.);
	n0=fnxco(origin_lat);
	phi1=origin_lat;

	do {
		BN=fnxco(phi1);
		nm=n0+lat1/GLMC_m0;
		error=(BN-nm)*pow((1-GLMC_ee*sin(phi1)*sin(phi1)), 1.5)/(GLMC_a*(1-GLMC_ee));
		phi1-=error;
	}while(fabs(error)>10e-10);

	nn= GLMC_ee/(1-GLMC_ee)*pow(cos(phi1),2.);
	M = GLMC_a*(1-GLMC_ee)/pow((1-GLMC_ee*pow(sin(phi1),2.)),1.5);
	N = GLMC_a/sqrt(1-GLMC_ee*pow(sin(phi1),2.));
	t=tan(phi1);

	lat1=phi1-lon1*lon1*t/(2*M*N*GLMC_m0*GLMC_m0)
		+pow(lon1, 4.)*t/(24*M*pow(N, 3.)+pow(GLMC_m0, 4.))*(5+3*t*t+nn-9*t*t*nn);
	lon1=lon1/(N*GLMC_m0*cos(phi1))-pow(lon1,3.)/(6*pow(N, 3.)*pow(GLMC_m0, 3.)*cos(phi1))*(1+2*t*t+nn)
		 +pow(lon1, 5)/(120*pow(N, 5.)*pow(GLMC_m0, 5.)*cos(phi1))*(5+28*t*t+24*pow(t, 4.));

	*lat=lat1*180./PI;
	*lon=lon1*180./PI+xy_lon0;
}

void TcrBsl2Tm(double *lat, double *lon, TCE_KTM_ORIGIN eOrigin)
{
	double b, e_prime, N, origin_lat, nn, t;
	double xy_lon0;
	double e, n, lat1, lon1;

	xy_lon0	= 0.0;

	lon1 = *lon;
	lat1 = *lat;

	if		( eOrigin == tmOriginWest	) { xy_lon0=125.+10.405/3600.;}
	if		( eOrigin == tmOriginMiddle ) { xy_lon0=127.+10.405/3600.;}
	if		( eOrigin == tmOriginEast	) { xy_lon0=129.+10.405/3600.;}

	origin_lat = GLMF_rad(38.0);

	lon1 -=xy_lon0;
	lon1 = GLMF_rad(lon1);
	lat1 = GLMF_rad(lat1);

	b = fnxco(lat1)-fnxco(origin_lat);
	N = GLMC_a/sqrt(1-GLMC_ee*pow(sin(lat1),2.));
	e_prime=GLMC_ee/(1-GLMC_ee);
	nn=e_prime*pow(cos(lat1),2.);
	t=tan(lat1);

	e = GLMC_m0*(lon1 * N* cos(lat1)+pow(lon1,3.)/6.*N*pow(cos(lat1),3.)*(1-t*t+nn)
		 +pow(lon1,5.)/120.*N*pow(cos(lat1),5.)*(5-18*t*t+pow(t,4.)));
	n = GLMC_m0*(b + pow(lon1, 2.)/2.*N*sin(lat1)*cos(lat1)
		 +pow(lon1,4.)/24.*N*sin(lat1)*pow(cos(lat1),3.)*(5-t*t+9*nn));

	*lat = n+GLMC_false_x;
	*lon = e+GLMC_false_y;
}

void TcrWgs2Bsl(double *lat, double *lon)
{
	int  dtx = 128,  /* unit:meter*/
		dty = -481,
		dtz = -664;
	double  Bessel_a = 6377397.155, /* Equatorial radius for Bessel meters */
		Bessel_f,  /* Flattening ratio for the Bessel Ellipse */
		wgs84_a = 6378137, /* W\Equatorial radius for Wgs84l meters */
		wgs84_f,  /* Flattening ratio for the Wgs84 Ellipse */
		wgs84_e2,
		dta,dtf,Q,L,S,Rn,Rm,dtlat,dtlon;

	Bessel_f = 1/299.1528128;
	wgs84_f = 1/298.257223563;
	dta = Bessel_a - wgs84_a ;
	dtf = Bessel_f - wgs84_f ;
	wgs84_e2 = (2 - wgs84_f) * wgs84_f;
	Q = (PI/180) * (*lat);      /* Y */
	L = (PI/180) * (*lon);      /* X */
	S = (PI / (180.0 * 3600.0) );    /* radian / b */
	Rn = wgs84_a / pow((1-wgs84_e2*(sin(Q)*sin(Q))),0.5);
	Rm = wgs84_a * (1-wgs84_e2) / pow((1-wgs84_e2*(sin(Q)*sin(Q))),1.5);
	/* dtlat,dtlon in radians */
	dtlat = ( dtz * cos(Q) - (dtx*sin(Q)*cos(L)) -  dty*sin(Q)*sin(L) +	((wgs84_a*dtf + wgs84_f*dta) * sin(2*Q)))/Rm;
	dtlon = ((dty* cos(L)) - (dtx*sin(L)))/(Rn * cos(Q));
	dtlat *= 180 / PI;
	dtlon *= 180 / PI;
	*lat = *lat + dtlat;
	*lon = *lon + dtlon;

	return ;
}





//{수정전. 20090209
//' GWS84 ellipse GWS 타원체
//double	AW;
//double	FW;
//}수정전. 20090209
//{수정후. 20090209
//}수정후. 20090209

//{수정전. 20090209
//' Bessel ellipse  Bessel 타원체
//const double	AB		= 6377397.155;
//const double	FB		= 1.0 / 299.152813;
//}수정전. 20090209
//{수정후. 20090209
//}수정후. 20090209

//' projection Datum  투영축척계수
const double	OKKTM	= 1.0 ;           //' 한국
const double	OKUTM	= 0.9996;         //' UTM

//' Moldensky의 좌표변환 parameters
//double	g_dTX, g_dTY, g_dTZ, g_dTOmega, g_dTPhi, g_dTKappa, g_dTS;
//int		g_nTMODE;
//double xb(30), yb(30), hB(30), wB(30);		//' geoid 고를 구하기 위한 점 데이터;
//double XW(30), YW(30), hW(30), wW(30);


TCS_COORD_ELLIPSOID_DAT	g_arEllipsoid[TCE_NUM_COORD_ELLIPSOID] = 
{
	{"Bessel 1841"	,	6377397.155	,	299.1528128		},	//TCE_COORD_ELLIPSOID_BESSEL1841
	{"WGS 84"		,	6378137.0	,	298.257223563	},	//TCE_COORD_ELLIPSOID_WGS1984
	{"GRS 80"		,	6378137.0	,	298.257222101	},	//TCE_COORD_ELLIPSOID_GRS80
};


TCS_COORD_PARAM_DAT		g_arTransParam[TCE_NUM_COORD_PARAM]	=
{
	//{"Tokyo Bessel"	, TCE_COORD_ELLIPSOID_BESSEL1841	,-147.0  ,  506.0  ,  687.0 ,"South Korea",2,2,2,29,TCE_COORD_TRANS_MOLODENSKY},	//TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA
	{"Tokyo Bessel"	, TCE_COORD_ELLIPSOID_BESSEL1841	,  128.0  , -481.0  , -664.0 ,"South Korea",0,0,0,0,TCE_COORD_TRANS_MOLODENSKY},	//TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA
	//{"Tokyo Bessel"	, TCE_COORD_ELLIPSOID_BESSEL1841	, 144.778, -503.702, -684.67,"South Korea",0,0,0, 0,TCE_COORD_TRANS_MOLODENSKY},	//TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA
	//                       타원체                      , 평행이동량 dx, dy, dz     이름  ,         회전량 오메가, 파이, 카파, ds(축척 변화량), 타원체 변화법
	{"고시2003-497"	, TCE_COORD_ELLIPSOID_GRS80			, -145.907,  505.034, -685.756,""           ,-1.162,2.347,1.592,6.342,TCE_COORD_TRANS_MOLODENSKY},	//TCE_COORD_PARAM_TM_GRS80_SOUTH_KOREA
	//{"Tokyo GRS80"	, TCE_COORD_ELLIPSOID_GRS80			, 144.778, -503.702, -684.67 ,""         ,0,0,0, 0,TCE_COORD_TRANS_MOLODENSKY},	//TCE_COORD_PARAM_TM_GRS80_SOUTH_KOREA
	//{"Tokyo GRS80"	, TCE_COORD_ELLIPSOID_GRS80			, 128.0  , -481.0  , -664.0 ,""           ,0,0,0, 0,TCE_COORD_TRANS_MOLODENSKY},	//TCE_COORD_PARAM_TM_GRS80_SOUTH_KOREA
};


void TcrBsl2GrsKtm(double* pLat, double* pLon, TCE_KTM_ORIGIN eOrigin)
{
	double	dLatW, dLonW, dHW;
	double	xb, yb;

	//베셀타원체 경위도를 GRS80타원체 경위도로 변환
	BSL41GPtoGRS80GP(*pLat, *pLon, 0.0, dLatW, dLonW, dHW);

	//GRS80타원체 경위도를 GRS80타원체의 TM좌표로 변환
	GRS80GPtoGRS80KTM(dLatW, dLonW, eOrigin, 0, xb, yb);

	*pLat	= xb - 304;	//북
	*pLon	= yb - 76;	//동
}

//void SetParam(TCE_COORD_PARAM eParam, int nTMode)
//{
//	TCS_COORD_PARAM_DAT*	pParam;
//
//	pParam	= g_arTransParam + eParam;
//
//	SetParam(pParam->dx, pParam->dy, pParam->dz, pParam->omega, pParam->phi, pParam->kappa, pParam->ds, nTMode);
//}
//
//void SetParam(double dx, double dy, double dz, double omega, double phi, double kappa, double ds, int nTMode)
//{
//	//'좌표변환 7개의 parameter를 설정함
//	//'dx,dy,dz : 변위 성분
//	//'omega, phi, kappa : 회전성분 (초 단위)
//	//'ds : 축척 (1e-6 단위임)
//	//'XB = (1+ts){R}XW + tx 임
//	//'imode : 변환방법
//	//'    1 : Molodensky 법
//	//'    2 : Bursa 법
//	//'프로젝트 파일 열거나 새 프로젝트 파일 만들때
//	//'자표변환설정에서 확인 눌렀을때
//	double	degrad;
//
//	degrad = atan(1.0) / 45.0;
//
//	g_dTX = dx;
//	g_dTY = dy;
//	g_dTZ = dz;
//	g_dTOmega = omega / 3600.0 * degrad;
//	g_dTPhi = phi / 3600.0 * degrad;
//	g_dTKappa = kappa / 3600.0 * degrad;
//	g_dTS = ds * 0.000001;
//	g_nTMODE = nTMode;
//
//	////' set Bessel Geoid data
//	//xb(1) = 34: yb(1) = 126:   hB(1) = -50: wB(1) = 3;
//	//xb(2) = 35: yb(2) = 126:   hB(2) = -55: wB(2) = 2;
//	//xb(3) = 36: yb(3) = 126:   hB(3) = -60: wB(3) = 2;
//	//xb(4) = 37: yb(4) = 126:   hB(4) = -64: wB(4) = 2;
//	//xb(5) = 38: yb(5) = 126:   hB(5) = -65: wB(5) = 3;
//	//xb(6) = 34: yb(6) = 127:   hB(6) = -46: wB(6) = 2;
//	//xb(7) = 35: yb(7) = 127:   hB(7) = -49: wB(7) = 1;
//	//xb(8) = 36: yb(8) = 127:   hB(8) = -56: wB(8) = 1;
//	//xb(9) = 37: yb(9) = 127:   hB(9) = -63: wB(9) = 1;
//	//xb(10) = 38: yb(10) = 127:   hB(10) = -65: wB(10) = 2;
//	//xb(11) = 34: yb(11) = 128:   hB(11) = -43: wB(11) = 2;
//	//xb(12) = 35: yb(12) = 128:   hB(12) = -45: wB(12) = 1;
//	//xb(13) = 36: yb(13) = 128:   hB(13) = -52: wB(13) = 1;
//	//xb(14) = 37: yb(14) = 128:   hB(14) = -58: wB(14) = 1;
//	//xb(15) = 38: yb(15) = 128:   hB(15) = -62: wB(15) = 2;
//	//xb(16) = 34: yb(16) = 129:   hB(16) = -42: wB(16) = 2;
//	//xb(17) = 35: yb(17) = 129:   hB(17) = -42: wB(17) = 1;
//	//xb(18) = 36: yb(18) = 129:   hB(18) = -47: wB(18) = 1;
//	//xb(19) = 37: yb(19) = 129:   hB(19) = -52: wB(19) = 1;
//	//xb(20) = 38: yb(20) = 129:   hB(20) = -60: wB(20) = 2;
//	//xb(21) = 34: yb(21) = 130:   hB(21) = -42: wB(21) = 3;
//	//xb(22) = 35: yb(22) = 130:   hB(22) = -42: wB(22) = 2;
//	//xb(23) = 36: yb(23) = 130:   hB(23) = -44: wB(23) = 2;
//	//xb(24) = 37: yb(24) = 130:   hB(24) = -48: wB(24) = 2;
//	//xb(25) = 38: yb(25) = 130:   hB(25) = -53: wB(25) = 3;
//
//	////' set WGS Geoid data
//	//XW(1) = 34: YW(1) = 126:   hW(1) = 24.5: wW(1) = 3;
//	//XW(2) = 35: YW(2) = 126:   hW(2) = 23.8: wW(2) = 2;
//	//XW(3) = 36: YW(3) = 126:   hW(3) = 23.3: wW(3) = 2;
//	//XW(4) = 37: YW(4) = 126:   hW(4) = 22.7: wW(4) = 2;
//	//XW(5) = 38: YW(5) = 126:   hW(5) = 22.9: wW(5) = 3;
//	//XW(6) = 34: YW(6) = 127:   hW(6) = 26.2: wW(6) = 2;
//	//XW(7) = 35: YW(7) = 127:   hW(7) = 26#: wW(7) = 1;
//	//XW(8) = 36: YW(8) = 127:   hW(8) = 25#: wW(8) = 1;
//	//XW(9) = 37: YW(9) = 127:   hW(9) = 24#: wW(9) = 1;
//	//XW(10) = 38: YW(10) = 127:   hW(10) = 23.8: wW(10) = 2;
//	//XW(11) = 34: YW(11) = 128:   hW(11) = 27.8: wW(11) = 2;
//	//XW(12) = 35: YW(12) = 128:   hW(12) = 28.5: wW(12) = 1;
//	//XW(13) = 36: YW(13) = 128:   hW(13) = 28#: wW(13) = 1;
//	//XW(14) = 37: YW(14) = 128:   hW(14) = 27#: wW(14) = 1;
//	//XW(15) = 38: YW(15) = 128:   hW(15) = 25.8: wW(15) = 2;
//	//XW(16) = 34: YW(16) = 129:   hW(16) = 28.8: wW(16) = 2;
//	//XW(17) = 35: YW(17) = 129:   hW(17) = 29.3: wW(17) = 1;
//	//XW(18) = 36: YW(18) = 129:   hW(18) = 29.7: wW(18) = 1;
//	//XW(19) = 37: YW(19) = 129:   hW(19) = 28.8: wW(19) = 1;
//	//XW(20) = 38: YW(20) = 129:   hW(20) = 27#: wW(20) = 2;
//	//XW(21) = 34: YW(21) = 130:   hW(21) = 29.3: wW(21) = 3;
//	//XW(22) = 35: YW(22) = 130:   hW(22) = 29.4: wW(22) = 2;
//	//XW(23) = 36: YW(23) = 130:   hW(23) = 29.4: wW(23) = 2;
//	//XW(24) = 37: YW(24) = 130:   hW(24) = 28.9: wW(24) = 2;
//	//XW(25) = 38: YW(25) = 130:   hW(25) = 27.8: wW(25) = 3;
//}

void WGS84GPtoKTM(double phiW, double lamW, double hW, TCE_KTM_ORIGIN eOrigin, double dLam, double& xb, double& yb, double& hB)
{
	//'WGS84 경위도를 한국의 직교좌표로 변환함
	//'입력
	//'phiW  : WGS 84의 위도
	//'lamW  : WGS 84의 경도
	//'hW    : WGS 84의 타원체 고
	//'imode : 변환방법
	//'    1 : Molodensky 법
	//'    2 : Bursa 법
	//'izone : 원점의 종류
	//'    0: 자동
	//'    1: 서부
	//'    2: 중부
	//'    3: 동부
	//          
	//'계산
	//'phiB  : BESSEL의 위도
	//'lamB  : BESSEL의 경도
	//'hB    : BESSEL의 타원체 고
	double phiB, lamB;

   //'WGS84 경위도를 한국의 경위도로 바꿈
   WGS84GPtoBSL41GP(phiW, lamW, hW, phiB, lamB, hB);
   //'한국의 경위도를 한국의 직교좌표로 바꿈
   BSL41GPtoKTM(phiB, lamB, eOrigin, dLam, xb, yb);
}

void KTMtoWGS84GP(double xb, double yb, double hB, TCE_KTM_ORIGIN eOrigin, double dLam, double& phiW, double& lamW, double& hW)
{
	//'한국의 직교좌표를 WGS84 경위도로 변환함
	//'입력
	//'phiB  : BESSEL의 위도
	//'lamB  : BESSEL의 경도
	//'hB    : BESSEL의 타원체 고
	//'izone : 원점의 종류
	//'     0: 자동
	//'     1: 서부
	//'     2: 중부
	//'     3: 동부
	//          
	//'계산
	//'phiW  : WGS 84의 위도
	//'lamW  : WGS 84의 경도
	//'hW    : WGS 84의 타원체 고
	double phiB, lamB;

	//'한국의 직교좌표를 한국의 경위도로 바꿈
	KTMtoBSL41GP(xb, yb, eOrigin, dLam, phiB, lamB);

	//'한국의 경위도를 WGS84 경위도로 바꿈
	BSL41GPtoWGS84GP(phiB, lamB, hB, phiW, lamW, hW);
}

void BSL41GPtoKTM(double phi, double lam, TCE_KTM_ORIGIN eOrigin, double dLam, double& X, double& Y)
{
	//'BESSEL 경위도를 한국의 직교좌표로 변환
	//'입력
	//'phi, lam : 위도 및 경도
	//'izone : 원점의 번호
	//'        0 - 자동으로 결정
	//'        1 - 서부원점 (38, 125)
	//'        2 - 중부원점 (38, 127)
	//'        3 - 동부원점 (38, 129)
	//'dlam : 경도에 더할 값, 초단위 (보통 10.405초 = 0.0028902777 도)
	//'계산
	//'x, y : 직교좌표 (x : 북,   y : 동)

	double dOrg;

	if ( tmOriginAuto == tmOriginAuto )
	{
	  if		( lam < 126.0 )	eOrigin = tmOriginWest;
	  else if	( lam > 128.0 )	eOrigin = tmOriginEast;
	  else						eOrigin	= tmOriginMiddle;
	}

	if		( eOrigin == tmOriginWest	)	dOrg	= 125.0;
	else if ( eOrigin == tmOriginMiddle )	dOrg	= 127.0;
	else if ( eOrigin == tmOriginEast	)	dOrg	= 129.0;

	//{수정전. 20090209
	//GP2TM(phi, lam, AB, FB, 500000.0, 200000.0, 38.0, 123 + izone * 2 + dLam / 3600.0, OKKTM, X, Y);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].a;
	double	FB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].f;
	GP2TM(phi, lam, AB, FB, 500000.0, 200000.0, 38.0, dOrg + dLam / 3600.0, OKKTM, X, Y);
	//}수정후. 20090209
}

void GRS80GPtoGRS80KTM(double phi, double lam, TCE_KTM_ORIGIN eOrigin, double dLam, double& X, double& Y)
{
	//'GRS80 경위도를 GRS80의 직교좌표로 변환
	//'입력
	//'phi, lam : 위도 및 경도
	//'izone : 원점의 번호
	//'        0 - 자동으로 결정
	//'        1 - 서부원점 (38, 125)
	//'        2 - 중부원점 (38, 127)
	//'        3 - 동부원점 (38, 129)
	//'dlam : 경도에 더할 값, 초단위 (보통 10.405초 = 0.0028902777 도)
	//'계산
	//'x, y : 직교좌표 (x : 북,   y : 동)

	double dOrg;

	if ( eOrigin == tmOriginAuto )
	{
	  if		( lam < 126.0 )	eOrigin = tmOriginWest;
	  else if	( lam > 128.0 )	eOrigin = tmOriginEast;
	  else						eOrigin	= tmOriginMiddle;
	}

	if		( eOrigin == tmOriginWest	)	dOrg	= 125.0;
	else if ( eOrigin == tmOriginMiddle )	dOrg	= 127.0;
	else if ( eOrigin == tmOriginEast	)	dOrg	= 129.0;

	//{수정전. 20090209
	//GP2TM(phi, lam, AB, FB, 500000.0, 200000.0, 38.0, 123 + izone * 2 + dLam / 3600.0, OKKTM, X, Y);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_GRS80].a;
	double	FB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_GRS80].f;
	//GP2TM(phi, lam, AB, FB, 500000.0, 200000.0, 38.0, dOrg + dLam / 3600.0, OKKTM, X, Y);
	GP2TM(phi, lam, AB, FB, 2000000.0, 1000000.0, 38.0, 127.50 , OKUTM, X, Y);
	//}수정후. 20090209
}

void KTMtoBSL41GP(double X, double Y, TCE_KTM_ORIGIN eOrigin, double dLam, double& phi, double& lam)
{
	//'한국의 직교좌표를 BESSEL 경위도로 변환
	//'입력
	//'x, y : 직교좌표 (x : 북,   y : 동)
	//'izone : 원점의 번호
	//'        0 - 자동으로 결정
	//'        1 - 서부원점 (38, 125)
	//'        2 - 중부원점 (38, 127)
	//'        3 - 동부원점 (38, 129)
	//'dlam : 경도에 더할 값, 초단위 (보통 10.405초 = 0.0028902777 도)
	//'계산
	//'phi, lam : 위도 및 경도

	double dOrg;

	if		( eOrigin == tmOriginWest	)	dOrg	= 125.0;
	else if ( eOrigin == tmOriginMiddle )	dOrg	= 127.0;
	else if ( eOrigin == tmOriginEast	)	dOrg	= 129.0;

	//{수정전. 20090209
	//TM2GP(X, Y, AB, FB, 500000.0, 200000.0, 38.0, 123 + izone * 2 + dLam / 3600.0, OKKTM, phi, lam);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].a;
	double	FB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].f;
	TM2GP(X, Y, AB, FB, 500000.0, 200000.0, 38.0, dOrg + dLam / 3600.0, OKKTM, phi, lam);
	//}수정후. 20090209
}

void BSL41GPtoGRS80GP(double phiB, double lamB, double hB, double& phiW, double& lamW, double& hW)
{
	//' 한국의 경위도를 GRS80 경위도로 변환함
	//' 입력
	//' phiB  : BESSEL의 위도
	//' lamB  : BESSEL의 경도
	//' hB    : BESSEL의 타원체 고
	//' imode : 변환방법
	//'     1 : Molodensky 법
	//'     2 : Bursa 법
	//       
	//' 계산
	//' phiW  : GRS80의 위도
	//' lamW  : GRS80의 경도
	//' hW    : GRS80의 타원체 고
	double XW, YW, zW, xb, yb, zB;

	//'먼저 베셀 경위도를 베셀 지심좌표로 변환한다
	BSL41GPtoBSL41CTR__(phiB, lamB, hB, xb, yb, zB);

	//'베셀 지심좌표를 GRS80 지심좌표로 변환한다.
	//BSL41CTRtoWGS84CTR(xb, yb, zb, XW, YW, zW);
	BSL41CTRtoGRS80CTR(xb, yb, zB, XW, YW, zW);

	//'GRS80 지심좌표를 GRS80 경위도로 변환한다
	GRS80CTRtoGRS80GP__(XW, YW, zW, phiW, lamW, hW);
}

void BSL41GPtoWGS84GP(double phiB, double lamB, double hB, double& phiW, double& lamW, double& hW)
{
	//' 한국의 경위도를 WGS84 경위도로 변환함
	//' 입력
	//' phiB  : BESSEL의 위도
	//' lamB  : BESSEL의 경도
	//' hB    : BESSEL의 타원체 고
	//' imode : 변환방법
	//'     1 : Molodensky 법
	//'     2 : Bursa 법
	//       
	//' 계산
	//' phiW  : WGS 84의 위도
	//' lamW  : WGS 84의 경도
	//' hW    : WGS 84의 타원체 고
	double XW, YW, zW, xb, yb, zB;

	//'먼저 베셀 경위도를 베셀 지심좌표로 변환한다
	BSL41GPtoBSL41CTR__(phiB, lamB, hB, xb, yb, zB);

	//'베셀 지심좌표를 WGS84 지심좌표로 변환한다.
	BSL41CTRtoWGS84CTR(xb, yb, zB, XW, YW, zW);

	//'WGS84 지심좌표를 WGS84경위도로 변환한다
	WGS84CTRtoWGS84GP__(XW, YW, zW, phiW, lamW, hW);
}

void WGS84GPtoBSL41GP(double phiW, double lamW, double hW, double& phiB, double& lamB, double& hB)
{
	//'WGS84 경위도를 한국의 경위도로 변환함
	//'입력
	//'phiW  : WGS 84의 위도
	//'lamW  : WGS 84의 경도
	//'hW    : WGS 84의 타원체 고
	//'imode : 변환방법
	//'    1 : Molodensky 법
	//'    2 : Bursa 법
	//       
	//'계산
	//'phiB  : BESSEL의 위도
	//'lamB  : BESSEL의 경도
	//'hB    : BESSEL의 타원체 고
	double XW, YW, zW, xb, yb, zB;

	//'먼저 WGS84경위도를 WGS84 지심좌표로 변환한다
	WGS84GPtoWGS84CTR__(phiW, lamW, hW, XW, YW, zW);
	//'WGS84 지심좌표를 베셀 지심좌표로 변환한다.
	WGS84CTRtoBSL41CTR(XW, YW, zW, xb, yb, zB);
	//'베셀 지심좌표를 베셀 경위도로 변환한다
	BCTR2BGP__(xb, yb, zB, phiB, lamB, hB);
}




void BSL41GPtoBSL41CTR__(double phi, double lam, double h, double& X, double& Y, double& Z)
{
	//'BESSEL 경위도를 BESSEL 지심좌표로 바꿈
	//'입력
	//'phi : 위도 (각도단위)
	//'lam : 경도
	//'h   : 타원체고
	//'계산
	//'X, Y, Z : 지심좌표

	//{수정전. 20090209
	//GP2CTR(phi, lam, h, AB, FB, X, Y, Z);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].a;
	double	FB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].f;
	GP2CTR__(phi, lam, h, AB, FB, X, Y, Z);
	//}수정후. 20090209
}

void BCTR2BGP__(double X, double Y, double Z, double& phi, double& lam, double& h)
{
	//'BESSEL 지심좌표를 BESSEL 경위도로 바꿈
	//'입력
	//'X, Y, Z : 지심좌표
	//'계산
	//'phi : 위도 (각도단위)
	//'lam : 경도
	//'h   : 타원체고
	//'**********************************

	//{수정전. 20090209
	//CTR2GP(X, Y, Z, AB, FB, phi, lam, h);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].a;
	double	FB	= g_arEllipsoid[TCE_COORD_ELLIPSOID_BESSEL1841].f;
	CTR2GP__(X, Y, Z, AB, FB, phi, lam, h);
	//}수정후. 20090209
}


void BSL41CTRtoWGS84CTR(double xb, double yb, double zB, double& XW, double& YW, double& zW)
{
	//'BESSEL 지심좌표를 WGS 지심좌표로 바꿈
	//'입력
	//'xb, yb, zb :BESSEL 지심좌표
	//'계산
	//'XW, YW, ZW :WGS 지심좌표

	TCS_COORD_PARAM_DAT*	pTransParam	= g_arTransParam+TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA;

	if		( pTransParam->method == TCE_COORD_TRANS_BURSA )
		InverseBursa__(pTransParam, xb, yb, zB, XW, YW, zW);
	else if ( pTransParam->method == TCE_COORD_TRANS_MOLODENSKY )
		InverseMolod__(pTransParam, xb, yb, zB, XW, YW, zW);
}

void BSL41CTRtoGRS80CTR(double xb, double yb, double zB, double& XW, double& YW, double& zW)
{
	//'BESSEL 지심좌표를 GRS80 지심좌표로 바꿈
	//'입력
	//'xb, yb, zb :BESSEL 지심좌표
	//'계산
	//'XW, YW, ZW :GRS80 지심좌표

	TCS_COORD_PARAM_DAT*	pTransParam	= g_arTransParam+TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA;

	if		( pTransParam->method == TCE_COORD_TRANS_BURSA )
		InverseBursa__(pTransParam, xb, yb, zB, XW, YW, zW);
	else if ( pTransParam->method == TCE_COORD_TRANS_MOLODENSKY )
		InverseMolod__(pTransParam, xb, yb, zB, XW, YW, zW);
}

void WGS84CTRtoBSL41CTR(double XW, double YW, double zW, double& xb, double& yb, double& zB)
{
	//'WGS 지심좌표를 BESSEL 지심좌표로 바꿈
	//'입력
	//'xw, yw, zw :WGS 지심좌표
	//'imode : 변환방법
	//'    1 : Molodensky 법
	//'    2 : Bursa 법
	//'계산
	//'xb, yb, zb :BESSEL 지심좌표

	TCS_COORD_PARAM_DAT*	pTransParam	= g_arTransParam+TCE_COORD_PARAM_TM_BESSEL_TOKYO_SOUTH_KOREA;

	if		( pTransParam->method == TCE_COORD_TRANS_BURSA )
		TransBursa__(pTransParam, XW, YW, zW, xb, yb, zB);
	else if ( pTransParam->method == TCE_COORD_TRANS_MOLODENSKY )
		TransMolod__(pTransParam, XW, YW, zW, xb, yb, zB);
}


void WGS84CTRtoWGS84GP__(double X, double Y, double Z, double& phi, double& lam, double& h)
{
	//'WGS 지심좌표를 WGS 경위도로 바꿈
	//'입력
	//'x, y, z : 지심좌표
	//'계산
	//'phi : 위도 (각도단위)
	//'lam : 경도
	//'h   : 타원체고

	//{수정전. 20090209
	//CTR2GP(X, Y, Z, AW, FW, phi, lam, h);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AW	= g_arEllipsoid[TCE_COORD_ELLIPSOID_WGS1984].a;
	double	FW	= g_arEllipsoid[TCE_COORD_ELLIPSOID_WGS1984].f;
	CTR2GP__(X, Y, Z, AW, FW, phi, lam, h);
	//}수정후. 20090209
}

void WGS84GPtoWGS84CTR__(double phi, double lam, double h, double& X, double& Y, double& Z)
{
	//' WGS 경위도를 WGS 지심좌표로 바꿈
	//' 입력
	//' phi : 위도 (각도단위)
	//' lam : 경도
	//' h   : 타원체고
	//' 계산
	//' X, Y, Z : 지심좌표

	//{수정전. 20090209
	//GP2CTR(phi, lam, h, AW, FW, X, Y, Z);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AW	= g_arEllipsoid[TCE_COORD_ELLIPSOID_WGS1984].a;
	double	FW	= g_arEllipsoid[TCE_COORD_ELLIPSOID_WGS1984].f;
	GP2CTR__(phi, lam, h, AW, FW, X, Y, Z);
	//}수정후. 20090209
}


void GRS80CTRtoGRS80GP__(double X, double Y, double Z, double& phi, double& lam, double& h)
{
	//'WGS 지심좌표를 WGS 경위도로 바꿈
	//'입력
	//'x, y, z : 지심좌표
	//'계산
	//'phi : 위도 (각도단위)
	//'lam : 경도
	//'h   : 타원체고

	//{수정전. 20090209
	//CTR2GP(X, Y, Z, AW, FW, phi, lam, h);
	//}수정전. 20090209
	//{수정후. 20090209
	double	AW	= g_arEllipsoid[TCE_COORD_ELLIPSOID_GRS80].a;
	double	FW	= g_arEllipsoid[TCE_COORD_ELLIPSOID_GRS80].f;
	CTR2GP__(X, Y, Z, AW, FW, phi, lam, h);
	//}수정후. 20090209
}



void GP2CTR__(double phi, double lam, double h, double a, double f1, double& X, double& Y, double& Z)
{
	//' 경위도를 지심좌표로 바꿈
	//' 입력 parameter
	//' phi  : 위도
	//' lam  : 경도
	//' a    : 장반경
	//' f    : 편평율 (1/299.....)
	//'계산 결과
	//' x, y, z : 지심좌표  x(north), y(east), z
	double degrad, sphi, slam, recf, b, es, n, f;
   
	f = f1;
	check(f);
	degrad = atan(1.0) / 45.0;
	sphi = phi * degrad;
	slam = lam * degrad;

	//'단반경 b, 횡곡률 반경 N의 계산
	//'** SEMI MAJOR AXIS - B **
	recf = 1.0 / f;
	b = a * (recf - 1.0) / recf;
	es = (a*a - b*b) / (a*a);

	//'횡곡률 반경
	n = fnSPHSN(a, es, sphi);

	//'x, y, z의 계산
	X = (n + h) * cos(sphi) * cos(slam);
	Y = (n + h) * cos(sphi) * sin(slam);
	Z = (((b*b) / (a*a)) * n + h) * sin(sphi);
}

void CTR2GP__(double X, double Y, double Z, double a, double f1, double& phi, double& lam, double& h)
{
	//'지심좌표를 경위도 좌표로 바꿈
	//'입력 parameter
	//'x#, y#, z# : 지심좌표  x(north), y(east), z
	//'a#    : 장반경
	//'f#    : 편평율 (1/299.....)
	//'계산 결과
	//'phi#  : 위도
	//'lam#  : 경도
	double	degrad, sphiold, sphinew, slam, recf, b, es, n, p, t1, f;
	int		icount;
	double	dTemp;

	//On Error GoTo CTRGPTransError
	f = f1;
	check(f);

	degrad = atan(1.0) / 45.0;


	//' 단반경 b, 횡곡률 반경 N의 계산
	//'     ** SEMI MAJOR AXIS - B **
	recf = 1.0 / f;
	b = a * (recf - 1.0) / recf;
	es = ((a*a) - (b*b)) / (a*a);

	//'경도 계산
	slam = atan(Y / X);

	//'위도 및 타원체 높이 계산
	p = sqrt(X * X + Y * Y);

	//'1차 추정값
	n = a;
	h = 0.0;
	sphiold = 0.0;
	//'오차가 10^-12이내 일때까지 반복계산
	icount = 0;
	while ( 1 )
	{
		icount = icount + 1;
		dTemp	= ((b*b) / (a*a) * n + h);
		t1 = (dTemp*dTemp) - (Z*Z);
		t1 = Z / sqrt(t1);
		sphinew = atan(t1);
		if ( abs(sphinew - sphiold) < 1E-18 )
			break;
		//' 새 횡곡률 반경 및 h
		n = fnSPHSN(a, es, sphinew);
		h = p / cos(sphinew) - n;
		sphiold = sphinew;
		if ( icount > 30 )
			break;
	};
	phi = sphinew / degrad;
	lam = slam / degrad;
	if ( X < 0.0 )
		lam = 180.0 + lam;		//' 90도 - 270 범위
	if ( lam < 0 )
		lam = 360.0 + lam;		//' 270도 - 360 범위
	//On Error GoTo 0
}




//void TransBursa(double XW, double YW, double zW, double& xb, double& yb, double& zB)
//{
//	//'WGS 지심좌표를 사용자 지심좌표로 변환함, Bursa법
//	//'입력
//	//'xw,yw,zw : WGS 지심좌표
//	//'계산
//	//'xb,yb,zb : 사용자 지심좌표
//	//
//	//'XB = (1+ts){R}XW + tx 임
//
//   xb = (1.0 + g_dTS) * (XW + g_dTKappa * YW - g_dTPhi * zW) + g_dTX;
//   yb = (1.0 + g_dTS) * (-g_dTKappa * XW + YW + g_dTOmega * zW) + g_dTY;
//   zB = (1.0 + g_dTS) * (g_dTPhi * XW - g_dTOmega * YW + zW) + g_dTZ;
//}

void TransBursa__(TCS_COORD_PARAM_DAT* pTransParam, double XW, double YW, double zW, double& xb, double& yb, double& zB)
{
	//'WGS 지심좌표를 사용자 지심좌표로 변환함, Bursa법
	//'입력
	//'xw,yw,zw : WGS 지심좌표
	//'계산
	//'xb,yb,zb : 사용자 지심좌표
	//
	//'XB = (1+ts){R}XW + tx 임

	xb = (1.0 + pTransParam->ds) * (XW + pTransParam->kappa * YW - pTransParam->phi * zW) + pTransParam->dx;
	yb = (1.0 + pTransParam->ds) * (-pTransParam->kappa * XW + YW + pTransParam->omega * zW) + pTransParam->dy;
	zB = (1.0 + pTransParam->ds) * (pTransParam->phi * XW - pTransParam->omega * YW + zW) + pTransParam->dz;
}

//void InverseBursa(double xb, double yb, double zB, double& XW, double& YW, double& zW)
//{
//	//'지심좌표를 사용자 WGS 지심좌표로 역 변환함, Bursa법
//	//'입력
//	//'xb,yb,zb : 사용자 지심좌표
//	//'계산
//	//'XW,YW,ZW : WGS 지심좌표
//	//
//	//'XW = 1/(1+ts) {R}^-1 {XB - tx} 임
//	double xt, yt, zt;
//
//	xt = xb - g_dTX;
//	yt = yb - g_dTY;
//	zt = zB - g_dTZ;
//
//	XW = 1.0 / (1.0 + g_dTS) * (xt - g_dTKappa * yt + g_dTPhi * zt);
//	YW = 1.0 / (1.0 + g_dTS) * (g_dTKappa * xt + yt - g_dTOmega * zt);
//	zW = 1.0 / (1.0 + g_dTS) * (-g_dTPhi * xt + g_dTOmega * yt + zt);
//}

void InverseBursa__(TCS_COORD_PARAM_DAT* pTransParam, double xb, double yb, double zB, double& XW, double& YW, double& zW)
{
	//'지심좌표를 사용자 WGS 지심좌표로 역 변환함, Bursa법
	//'입력
	//'xb,yb,zb : 사용자 지심좌표
	//'계산
	//'XW,YW,ZW : WGS 지심좌표
	//
	//'XW = 1/(1+ts) {R}^-1 {XB - tx} 임
	double xt, yt, zt;

	xt = xb - pTransParam->dx;
	yt = yb - pTransParam->dy;
	zt = zB - pTransParam->dz;

	XW = 1.0 / (1.0 + pTransParam->ds) * (xt - pTransParam->kappa * yt + pTransParam->phi * zt);
	YW = 1.0 / (1.0 + pTransParam->ds) * (pTransParam->kappa * xt + yt - pTransParam->omega * zt);
	zW = 1.0 / (1.0 + pTransParam->ds) * (-pTransParam->phi * xt + pTransParam->omega * yt + zt);
}

//void TransMolod(double XW, double YW, double zW, double& xb, double& yb, double& zB)
//{
//	//'WGS 지심좌표를 사용자 지심좌표로 변환함, Molodensky법
//	//'입력
//	//'xw,yw,zw : WGS 지심좌표
//	//'계산
//	//'xb,yb,zb : 사용자 지심좌표
//	//
//	//'XB = (1+ts){R}XW + tx 임
//
//	xb = XW + (1.0 + g_dTS) * (g_dTKappa * YW - g_dTPhi * zW) + g_dTX;
//	yb = YW + (1.0 + g_dTS) * (-g_dTKappa * XW + g_dTOmega * zW) + g_dTY;
//	zB = zW + (1.0 + g_dTS) * (g_dTPhi * XW - g_dTOmega * YW) + g_dTZ;
//}

void TransMolod__(TCS_COORD_PARAM_DAT* pTransParam, double XW, double YW, double zW, double& xb, double& yb, double& zB)
{
	//'WGS 지심좌표를 사용자 지심좌표로 변환함, Molodensky법
	//'입력
	//'xw,yw,zw : WGS 지심좌표
	//'계산
	//'xb,yb,zb : 사용자 지심좌표
	//
	//'XB = (1+ts){R}XW + tx 임

	xb = XW + (1.0 + pTransParam->ds) * (pTransParam->kappa * YW - pTransParam->phi * zW) + pTransParam->dx;
	yb = YW + (1.0 + pTransParam->ds) * (-pTransParam->kappa * XW + pTransParam->omega * zW) + pTransParam->dy;
	zB = zW + (1.0 + pTransParam->ds) * (pTransParam->phi * XW - pTransParam->omega * YW) + pTransParam->dz;
}

//void InverseMolod(double xb, double yb, double zB, double& XW, double& YW, double& zW)
//{
//	//'지심좌표를 사용자 WGS 지심좌표로 역 변환함, Molodensky법
//	//'입력
//	//'xb,yb,zb : 사용자 지심좌표
//	//'계산
//	//'XW,YW,ZW : WGS 지심좌표
//
//	//'XW = 1/(1+ts) {R}^-1 {XB - tx} 임
//	double xt, yt, zt;
//
//	xt = (xb - g_dTX) * (1.0 + g_dTS);
//	yt = (yb - g_dTY) * (1.0 + g_dTS);
//	zt = (zB - g_dTZ) * (1.0 + g_dTS);
//
//	XW = 1.0 / (1.0 + g_dTS) * (xt - g_dTKappa * yt + g_dTPhi * zt);
//	YW = 1.0 / (1.0 + g_dTS) * (g_dTKappa * xt + yt - g_dTOmega * zt);
//	zW = 1.0 / (1.0 + g_dTS) * (-g_dTPhi * xt + g_dTOmega * yt + zt);
//}

void InverseMolod__(TCS_COORD_PARAM_DAT* pTransParam, double xb, double yb, double zB, double& XW, double& YW, double& zW)
{
	//'지심좌표를 사용자 WGS 지심좌표로 역 변환함, Molodensky법
	//'입력
	//'xb,yb,zb : 사용자 지심좌표
	//'계산
	//'XW,YW,ZW : WGS 지심좌표

	//'XW = 1/(1+ts) {R}^-1 {XB - tx} 임
	double xt, yt, zt;

	xt = (xb - pTransParam->dx) * (1.0 + pTransParam->ds);
	yt = (yb - pTransParam->dy) * (1.0 + pTransParam->ds);
	zt = (zB - pTransParam->dz) * (1.0 + pTransParam->ds);

	XW = 1.0 / (1.0 + pTransParam->ds) * (xt - pTransParam->kappa * yt + pTransParam->phi * zt);
	YW = 1.0 / (1.0 + pTransParam->ds) * (pTransParam->kappa * xt + yt - pTransParam->omega * zt);
	zW = 1.0 / (1.0 + pTransParam->ds) * (-pTransParam->phi * xt + pTransParam->omega * yt + zt);
}



void GP2TM(double phi, double lam, double a, double f1, double x0, double y0, double phi0, double lam0, double ok, double& xn, double& ye)
{
	//'
	//' ----------------------------------------------------------------------------
	//' ?FILE NAME: gp2utm.bas        VERSION: 9102.07      DESIGN DATE:  6/16/87 ?
	//' ?STRUCTURE: procedure         PURPOSE: Compute UTM grid from GP           ?
	//' ?LANGUAGE : Power Basic       AUTHOR : B. W. Drew                         ?
	//' 한국및 일반적인 TM좌표변환에 사용될 수 있도록 고침
	//' zone을 없애고, 원점의 좌표및 축척계수를 입력받아 계산함 99.12
	//' 지오그룹 장찬수 561-3131
	//'-----------------------------------------------------------------------------
	//'
	//' ?                           DOCUMENTATION                                 ?
	//'-----------------------------------------------------------------------------
	//' FUNCTIONAL DESCRIPTION:  Converts geographic (latitude, longitude)
	//' coordinates to grid coordinates (zone, northing, easting) on the universal
	//' transverse Mercator (UTM) projection. ACCURACY NOTE: Terms T4, T5, T8 & T9
	//' may not be needed in applications requiring less accuracy.
	//'
	//' CALLING SEQUENCE:  call GP2UTM (A, F, SPHI, SLAM, IZONE, xn, ye, IFIXZ)
	//' 99. 12수정     GP2TM(PHI, LAM, a, f, x0, y0, phi0, lam0, ok, xn, ye)
	//' INPUT ARGUEMENTS:
	//'  Variable Name      Type  Description
	//'  ------------------------------------
	//'  phi              dbl   latitude, radians 위도- 99.12 각도로 읽음
	//'  lam              dbl   longitude, radians 경도- 99.12 각도로 읽음
	//'  a                 dbl   semi-major axis of ellipsoid, meters
	//'  f                 dbl   flattening of ellipsoid
	//'  izone              int   UTM zone number - 99.12 삭제
	//'  ifixz              bool  Flag, = 0 compute UTM zone, = 1 use input zone - 99.12 삭제
	//' 99. 12 추가
	//' x0, y0 : 원점의 좌표값
	//' phi0, lam0 : 원점의 위도, 경도
	//' phi, lam : 변환하고자 하는 점의 위도, 경도
	//' ok : 원점의 축척계수
	//'
	//' OUTPUT ARGUEMENTS:
	//'  Variable Name      Type  Description
	//'  -------------------------------------
	//'  izone              int   UTM zone number (see input arguements) - 99.12 삭제
	//'  xn                 dbl   UTM northing, meters
	//'  ye                 dbl   UTM easting, meters
	//'  99.12 주) 원래는 x가 동측, y가 북측이었으나 우리의 관습에 따라 xn을 북쪽으로 고침
	//'
	//'
	//'
	//' FILES/DEVICES:  none
	//'
	//' ---------------------------------------------------------------------------
	//' ?                              REVISIONS                                  ?
	//' ---------------------------------------------------------------------------
	//' REVISION   :  9102.07           AUTHOR:  R. E. Ziegler
	//' DESCRIPTION:  Convert original FORTRAN to Power Basic.
	//' 99. 12 visual basic 으로 고침, 장찬수
	//' ---------------------------------------------------------------------------
	//'
	//'
	//' 99.12 DIMEMSION
	double sphi, slam, sphi0, slam0, f;
	double fe, degrad, recf, b, es, ebs, tn;
	double ap, bp, cp, dp, ep;
	double dLam, s, c, c2m, c3m, c5m, c7m, t, eta, sn, tmd, tmd1, nfn, xn1;
	double t1, t2, t3, t4, t5, t6, t7, t8, t9;

	double tn2m, tn3m, tn4m, tn5m;
	double t2m, t4m, t6m;
	double eta2m, eta3m, eta4m;
	double dLam2m, dLam3m, dLam4m, dLam5m, dLam6m, dLam7m, dLam8m;

	//On Error GoTo TMTransError
	//'99.12
	//'FE = 500000
	fe = y0;      //' 원점의 x좌표
	f = f1;       //' 편평도
	check(f);       //' 편평도가 299.1528... 로 입력 되었으면 0.003342....로 바꿈
	
	//ok = 0.9996   ' 축척계수, 고정하지 않고 파라미터로 전달받음 99.12

	degrad = atan(1.0) / 45.0;
	sphi = phi * degrad;		//'위도를 라디안으로
	slam = lam * degrad;
	sphi0 = phi0 * degrad;	//'원점의 위도를 라디안으로
	slam0 = lam0 * degrad;
	//'
	//'    *************************************************
	//'    *****   DERIVE OTHER ELLIPSOID PARAMTERS    *****
	//'    *****         FROM INPUT VARIABLES          *****
	//'    *****    A = SEMI-MAJOR AXIS OF ELLIPSOID   *****
	//'    ***** RECF = RECIPROCAL OF FLATTENING (1/F) *****
	//'    *************************************************
	//'
	//'
	//'     ** SEMI MAJOR AXIS - B **
	recf = 1.0 / f;                       //' 편평도 299.1528...
	b = a * (recf - 1.0) / recf;
	//'
	//'     ** ECCENTRICITY SQUARED **
	es = ((a*a) - (b*b)) / (a*a);
	//'
	//'     ** SECOND ECCENTRICITY SQUARED **
	ebs = ((a*a) - (b*b)) / (b*b);
	//'
	//'     ** TRUE MERIDIONAL DISTANCE CONSTANTS **
	tn = (a - b) / (a + b);
	tn2m= tn*tn;
	tn3m= tn2m*tn;
	tn4m= tn3m*tn;
	tn5m= tn4m*tn;
	ap = a * (1.0 - tn + 5.0 * (tn2m - tn3m) / 4.0 + 81.0 * (tn4m - tn5m) / 64.0);
	bp = 3.0 * a * (tn - tn2m + 7.0 * (tn3m - tn4m) / 8.0 + 55.0 * tn5m / 64.0) / 2.0;
	cp = 15.0 * a * (tn2m - tn3m + 3.0 * (tn4m - tn5m) / 4.0) / 16.0;
	dp = 35.0 * a * (tn3m - tn4m + 11.0 * tn5m / 16.0) / 48.0;
	ep = 315.0 * a * (tn4m - tn5m) / 512.0;
	//'
	//'     ***** ZONE - CENTRAL MERIDIAN *****
	//'
	//'     *** ZONE ***
	//'
	//'     *** HOLD FIXED IF IFIXZ IS SET TO ONE
	//' 99.12 아래 zone 계산 삭제
	//'     *** DETERMINE ZONE NUMBER IF IFIXZ IS ZERO
	//'    If IFIXZ = 0 Then
	//'     ***
	//'       IZONE = 31 + Int(SLAM / DEGRAD / 6)
	//'
	//'         ** ZONE TRAP - AT HEMISPHERE LIMITS **
	//'       If IZONE > 60 Then IZONE = 60
	//'       If IZONE < 1 Then IZONE = 1
	//'    End If
	//'
	//'     *** CENTRAL MERIDIAN ***
	//'    OLAM = (IZONE * 6 - 183) * DEGRAD
	//'
	//'     *** DELTA LONGITUDE ***
	//'     *** DIFFERENCE BETWEEN LONGITUDE AND CENTRAL MERIDIAN ***
	dLam = slam - slam0;     //' 원점에서 떨어진 경도 rad
	dLam2m= dLam*dLam;
	dLam3m= dLam2m*dLam;
	dLam4m= dLam2m*dLam2m;
	dLam5m= dLam3m*dLam2m;
	dLam6m= dLam4m*dLam2m;
	dLam7m= dLam4m*dLam3m;
	dLam8m= dLam4m*dLam4m;

	//'
	//' *** 원점에 대한 x 좌표 nfn을 먼저 구함    99.12
	//'     ** TRUE MERIDIONAL DISTANCE **

	tmd1 = fnSPHTMD(ap, bp, cp, dp, ep, sphi0);
	nfn = tmd1 * ok;

	//' ****  구하는 점의 x 좌표를 구함
	//'     *** OTHER COMMON TERMS ***
	s = sin(sphi);
	c = cos(sphi);
	c2m= c*c;
	c3m= c2m*c;
	c5m= c3m*c2m;
	c7m= c5m*c2m;
	t = s / c;
	t2m= t*t;
	t4m= t2m*t2m;
	t6m= t4m*t2m;
	eta = ebs * c2m;
	eta2m=eta*eta;
	eta3m=eta2m*eta;
	eta4m=eta3m*eta;
	//'
	//'     ** RADIUS OF CURVATURE IN PRIME VERTICAL **
	sn = fnSPHSN(a, es, sphi);
	//'
	//'     ** TRUE MERIDIONAL DISTANCE **
	tmd = fnSPHTMD(ap, bp, cp, dp, ep, sphi);
	//'
	//'     ***** NORTHING *****
	//'
	t1 = tmd * ok;
	t2 = sn * s * c * ok / 2.0;
	t3 = sn * s * c3m * ok * (5.0 - t2m + 9.0 * eta + 4.0 * eta2m) / 24.0;
	t4 = sn * s * c5m * ok * (61.0 - 58.0 * t2m + t4m + 270.0 * eta - 330.0 * t2m * eta
	   + 445.0 * eta2m + 324.0 * eta3m - 680.0 * t2m * eta2m + 88.0 * eta4m
	   - 600.0 * t2m * eta3m - 192.0 * t2m * eta4m) / 720.0;
	t5 = sn * s * c7m * ok * (1385.0 - 3111.0 * t2m + 543.0 * t4m - t6m) / 40320.0;
	//'
	//'     ** FALSE NORTHING **
	//'    nfn = 0
	//'    If Sgn(SPHI) = -1 Then nfn = 10000000
	//'
	xn1 = t1 + dLam2m * t2 + dLam4m * t3 + dLam6m * t4 + dLam8m * t5;
	xn = xn1 - nfn + x0; //' 원점과의 차이를 구함   99.12


	//'
	//'     ***** EASTING *****
	//'
	t6 = sn * c * ok;
	t7 = sn * c3m * ok * (1.0 - t2m + eta) / 6.0;
	t8 = sn * c5m * ok * (5.0 - 18.0 * t2m + t4m + 14.0 * eta - 58.0 * t2m * eta
		  + 13.0 * eta2m + 4.0 * eta3m - 64.0 * t2m * eta2m - 24.0 * t2m * eta3m) / 120.0;
	t9 = sn * c7m * ok * (61.0 - 479.0 * t2m + 179.0 * t4m - t6m) / 5040.0;
	//'
	ye = fe + dLam * t6 + dLam3m * t7 + dLam5m * t8 + dLam7m * t9;
	//On Error GoTo 0
	//Exit Sub
	//
	//TMTransError:
	//   MsgBox "직교좌표로 변환할 수 없는 데이터 입니다. 타원체 제원과 경위도를 확인후 다시 하십시오"
	//   xn = x0
	//   ye = y0
	//   On Error GoTo 0
}

void TM2GP(double xn, double ye, double a, double f1, double x0, double y0, double phi0, double lam0, double ok, double& phi, double& lam)
{
	//'
	//' ----------------------------------------------------------------------------
	//' ?FILE NAME: utm2gp.bas       VERSION : 9102.07      DESIGN DATE:  7/06/88 ?
	//' ?STRUCTURE: Subroutine       AUTHOR  : B. W. Drew                         ?
	//' ?LANGUAGE : Basic            COMPILER: PowerBASIC       VERSION: 2.10a    ?
	//' ?PURPOSE  : Convert UTM to GP                                             ?
	//' 한국및 일반적인 TM좌표변환에 사용될 수 있도록 고침
	//' zone을 없애고, 원점의 좌표및 축척계수를 입력받아 계산함 99.12
	//' 지오그룹 장찬수 561-3131
	//' ----------------------------------------------------------------------------
	//'
	//' ----------------------------------------------------------------------------
	//' ?                           DOCUMENTATION                                 ?
	//' ----------------------------------------------------------------------------
	//' FUNCTIONAL DESCRIPTION: CONVERTS GRID COORDINATES (ZONE, NORTHING, EASTING ) ***
	//'  TO GEOGRAPHIC COORDINATES (LATITUDE, LONGITUDE) ON THE UNIVERSAL TRANSVERSE
	//'  MERCATOR (UTM) GRID.
	//'
	//' CALLING SEQUENCE:  call UTM2GP(A, F, SPHI, SLAM, IZONE, xn, ye)
	//'
	//' INPUT ARGUEMENTS:
	//'  Variable Name      Type  Description
	//' ----------------------------------------------
	//'  a                 dbl   Ellipsoid semi-major axis, meters
	//' f                 dbl   Ellipsoid flattening
	//' izone              int   UTM zone  - 삭제 99.12
	//' xn                 dbl   UTM northing, meters
	//' ye                 dbl   UTM easting, meters
	//'
	//' 99. 12 추가
	//' x0, y0 : 원점의 좌표값
	//' phi0, lam0 : 원점의 위도, 경도
	//' ok : 원점의 축척계수
	//
	//' OUTPUT ARGUEMENTS:
	//'  Variable Name      Type  Description
	//'  ---------------------------------------------
	//'  sphi              dbl   Latitude, radians
	//' slam              dbl   Longitude, radians
	//'
	//' FILES/DEVICES: none
	//' ----------------------------------------------------------------------------
	//' ?                              REVISIONS                                  ?
	//' ----------------------------------------------------------------------------
	//' REVISION    : yymm.dd           AUTHOR: R. E. Ziegler
	//' ORGANIZATION: x
	//' DESCRIPTION : x
	//' 99. 12 visual basic 으로 고침, 장찬수
	//' ----------------------------------------------------------------------------
	//'
	//'99.12 DIMEMSION
	double	sphi, slam, sphi0, slam0, f;
	double	fe, degrad, recf, b, es, ebs, tn;
	double	ap, bp, cp, dp, ep;
	double	dLam, s, c, t, eta, sn, tmd, tmd1, nfn, xn1;
	double	t10, t11, t12, t13, t14, t15, t16, t17;
	double	de, sr, ftphi;
	int		i;

	double	tn2m, tn3m, tn4m, tn5m;
	double	t2m, t4m, t6m;
	double	eta2m, eta3m, eta4m;
	double	sn3m, sn5m, sn7m;
	double	ok2m, ok3m, ok4m, ok5m, ok6m, ok7m, ok8m;
	double	de2m, de3m, de4m, de5m, de6m, de7m, de8m;

	ok2m= ok*ok;
	ok3m= ok2m*ok;
	ok4m= ok2m*ok2m;
	ok5m= ok3m*ok2m;
	ok6m= ok4m*ok2m;
	ok7m= ok4m*ok3m;
	ok8m= ok4m*ok4m;

	//On Error GoTo GPTransError
	f = f1;
	check(f);
	//'
	fe = y0;
	degrad = atan(1.0) / 45.0;
	sphi0 = phi0 * degrad;
	slam0 = lam0 * degrad;

	//'
	//'    *************************************************
	//'    *****   DERIVE OTHER ELLIPSOID PARAMTERS    *****
	//'    *****         FROM INPUT VARIABLES          *****
	//'    *****    A = SEMI-MAJOR AXIS OF ELLIPSOID   *****
	//'    ***** RECF = RECIPROCAL OF FLATTENING (1/F) *****
	//'    *************************************************
	//'
	//'
	//'     ** SEMI MAJOR AXIS - B **
	recf = 1.0 / f;
	b = a * (recf - 1.0) / recf;
	//'
	//'     ** ECCENTRICITY SQUARED **
	es = ((a*a) - (b*b)) / (a*a);
	//'
	//'     ** SECOND ECCENTRICITY SQUARED **
	ebs = ((a*a) - (b*b)) / (b*b);
	//'
	//'     ** TRUE MERIDIONAL DISTANCE CONSTANTS **
	tn = (a - b) / (a + b);
	tn2m=tn*tn;
	tn3m=tn2m*tn;
	tn4m=tn2m*tn2m;
	tn5m=tn3m*tn2m;
	ap = a * (1.0 - tn + 5.0 * (tn2m - tn3m) / 4.0 + 81.0 * (tn4m - tn5m) / 64.0);
	bp = 3.0 * a * (tn - tn2m + 7.0 * (tn3m - tn4m) / 8.0 + 55.0 * tn5m / 64.0) / 2.0;
	cp = 15.0 * a * (tn2m - tn3m + 3.0 * (tn4m - tn5m) / 4.0) / 16.0;
	dp = 35.0 * a * (tn3m - tn4m + 11.0 * tn5m / 16.0) / 48.0;
	ep = 315.0 * a * (tn4m - tn5m) / 512.0;
	//'
	//'     *** HEMISPHERE ADJUSTMENT TO FALSE NORTHING & POINT NORTHING ***
	//'     *   NORTHERN HEMISPHERE
	//'    nfn = 0
	//'    If Sgn(xn) < 0 Then
	//'      nfn = 10000000
	//'      xn = Abs(xn)
	//'    End If
	//'
	//' *** 원점에 대한 x 좌표 nfn을 먼저 구함    99.12
	//'     ** TRUE MERIDIONAL DISTANCE **
	tmd1 = fnSPHTMD(ap, bp, cp, dp, ep, sphi0);
	nfn = tmd1 * ok;

	//'     ** TRUE MERIDIONAL DISTANCE FOR FOOTPOINT LATITUDE **
	//    
	//'    tmd = (xn - nfn) / ok       '99.12
	 xn1 = xn + nfn - x0;
	 tmd = xn1 / ok;
	//'
	//'     ***** FOOTPOINT LATITUDE *****
	//'
	//'     ** 1ST ESTIMATE **
	sr = fnSPHSR(a, es, 0.0);
	ftphi = tmd / sr;
	//'
	//'     ******************************************
	//'     ** ITERATE TO OBTAIN FOOTPOINT LATITUDE **
	//'
	for ( i = 1; i <= 5; i++ )
	{
	//'
	//'    * COMPUTED TRUE MERIDIONAL *
		t10 = fnSPHTMD(ap, bp, cp, dp, ep, ftphi);
	//'
	//'     * COMPUTED RADIUS OF CURVATURE IN THE MERIDIAN *
		sr = fnSPHSR(a, es, ftphi);
	//'
	//'     * CORRECTED FOOTPOINT LATITUDE *
	//'     * NEW FTPOINT = LAST FTPOINT +(TMDACTUAL -TMDCOMP)/RADIUS
		ftphi = ftphi + (tmd - t10) / sr;
	}
	//'
	//'     ******************************************
	//'
	//'     ** RADIUS OF CURVATURE IN THE MERIDIAN **
	sr = fnSPHSR(a, es, ftphi);
	//'
	//'     ** RADIUS OF CURVATURE IN PRIME VERTICAL **
	sn = fnSPHSN(a, es, ftphi);
	sn3m= sn*sn*sn;
	sn5m= sn3m*sn*sn;
	sn7m= sn5m*sn*sn;
	//'
	//'     ** OTHER COMMON TERMS **
	s = sin(ftphi);
	c = cos(ftphi);
	t = s / c;
	t2m= t*t;
	t4m= t2m*t2m;
	t6m= t4m*t2m;
	eta = ebs * (c*c);
	eta2m= eta*eta;
	eta3m= eta2m*eta;
	eta4m= eta2m*eta2m;
	//'
	//'     ** DELTA EASTING - DIFFERENCE IN EASTING **
	de = ye - fe;
	de2m= de*de;
	de3m= de2m*de;
	de4m= de2m*de2m;
	de5m= de3m*de2m;
	de6m= de4m*de2m;
	de7m= de4m*de3m;
	de8m= de4m*de4m;
	//'     *******************************
	//'
	//'
	//'     ***** LATITUDE *****
	//'
	t10 = t / (2.0 * sr * sn * ok2m);
	t11 = t * (5.0 + 3.0 * t2m + eta - 4.0 * eta2m - 9.0 * t2m * eta) / (24.0 * sr * sn3m * ok4m);
	t12 = t * (61.0 + 90.0 * t2m + 46.0 * eta + 45.0 * t4m - 252.0 * t2m * eta - 3.0 * eta2m
		   + 100.0 * eta3m - 66.0 * t2m * eta2m - 90.0 * t4m * eta + 88.0 * eta4m
		   + 225.0 * t4m * eta2m + 84.0 * t2m * eta3m - 192.0 * t2m * eta4m) / (720.0 * sr * sn5m * ok6m);
	t13 = t * (1385.0 + 3633.0 * t2m + 4095.0 * t4m + 1575.0 * t6m) / (40320.0 * sr * sn7m * ok8m);
	//'
	//'     ** LATITUDE **
	sphi = ftphi - de2m * t10 + de4m * t11 - de6m * t12 + de8m * t13;
	//'
	//'     ***** LONGITUDE *****
	//'
	t14 = 1.0 / (sn * c * ok);
	t15 = (1.0 + 2.0 * t2m + eta) / (6.0 * sn3m * c * ok3m);
	t16 = (5.0 + 6.0 * eta + 28.0 * t2m - 3.0 * eta2m + 8.0 * t2m * eta + 24.0 * t4m
		   - 4.0 * eta3m + 4.0 * t2m * eta2m + 24.0 * t2m * eta3m) / (120.0 * sn5m * c * ok5m);
	t17 = (61.0 + 662.0 * t2m + 1320.0 * t4m + 720.0 * t6m) / (5040.0 * sn7m * c * ok7m);
	//'
	//'     ** DIFFERENCE IN LONGITUDE **
	dLam = de * t14 - de3m * t15 + de5m * t16 - de7m * t17;
	//'
	//'     ** CENTRAL MERIDIAN **
	//'    olam = (IZONE * 6 - 183) * DEGRAD    ' 99.12
	//'
	//'     ** LONGITUDE **
	//'    slam = olam + dlam
	 slam = slam0 + dLam;		//' 99.12
	//' change to degree unit

	phi = sphi / degrad;
	lam = slam / degrad;
	//'
	//'     *** ACCURACY NOTE ***
	//'     *** TERMS T12, T13, T16 & T17 MAY NOT BE NEEDED IN
	//'     *** APPLICATIONS REQUIRING LESS ACCURACY
	//'
	//On Error GoTo 0
	//Exit Sub

	//GPTransError:
	//MsgBox "경위도로 변환할 수 없는 직교좌표 입니다. 타원체 제원과 좌표를 확인후 다시 하십시오"
	//phi = phi0
	//lam = lam0
	//On Error GoTo 0
}


void check(double& f)
{
	//' 편평도가 299.1528... 로 입력 되었으면 0.003342....로 바꿈
    if ( f > 1.0)
		f = 1.0 / f;
}


double fnSPHSN(double a, double es, double sphi)
{
	//'
	//'     *** RADIUS OF CURVATURE IN THE PRIME VERTICAL FROM LATITUDE ***
	//'
	double fnSPHSN;

	fnSPHSN = a / sqrt(1.0 - es * (sin(sphi)*sin(sphi)));
	return fnSPHSN;
}

double fnSPHTMD(double ap, double bp, double cp, double dp, double ep, double sphi)
{
	//'     *** TRUE MERIDIONAL DISTANCE FROM LATITUDE ***
	//'
	double fnSPHTMD;

	fnSPHTMD = ap * sphi - bp * sin(2.0 * sphi)
			 + cp * sin(4.0 * sphi) - dp * sin(6.0 * sphi) + ep * sin(8.0 * sphi);
	return fnSPHTMD;
}

double fnSPHSR(double a, double es, double sphi)
{
	//'     *** RADIUS OF CURVATURE IN THE MERIDIAN FROM LATITUDE ***
	//'
	double fnSPHSR;
	double denom;

	denom	= fnDENOM(es, sphi);
	fnSPHSR = a * (1.0 - es) / (denom*denom*denom);
	return fnSPHSR;
}

double fnDENOM(double es, double sphi)
{
	//'     *** RADIUS OF CURVATURE IN THE MERIDIAN FROM LATITUDE ***
	//'
	double fnDENOM;
	double sin_sphi;

	sin_sphi	= sin(sphi);
	fnDENOM = sqrt(1.0 - es * (sin_sphi*sin_sphi));
	return fnDENOM;
}
