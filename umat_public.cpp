// #include <omi_for_c.h>
#include <Eigen/Eigen>

void CalcElaStiffMat(	Eigen::Ref<Eigen::Matrix<double, 6,6>> C, 
						const double &G, 
						const double &LAMBDA)
{
    C = Eigen::Matrix<double, 6,6>::Zero();
    // Kelvin Voigt
    for (int i = 0; i < 3; i++)
    {
		C(i,i) += 2*G;
        C(i+3,i+3) += G;
        for (int j = 0; j < 3; j++)
        {
            C(i,j) += LAMBDA;

        }   
    } 
}

extern "C" void umat(double *stress, double *statev, double *ddsdde, double *sse, double *spd,
		double *scd, double *rpl, double *ddsddt, double *drplde, double *drpldt,
		double *stran, double *dstran, double *time, double *dtime, double *temp,
		double *dtemp, double *predef, double *dpred, char *cmname, int *ndi,
		int *nshr, int *ntens, int *nstatv, double *props, int *nprops, 
		double *coords, double *drot, double *pnewdt, double *celent, double *dfgrd0, 
		double *dfgrd1, int *noel, int *npt, int *layer, int *kspt, 
		int *kstep, int *kinc, short cmname_len)
{
	double E = props[0]; //210000
	double NU = props[1]; //0.3
	double G = E/2.0/(1.0 + NU); //80769,23
	double LAMBDA = 2.0*G*NU/(1.0 - 2.0*NU);//121153,85
	double eps [6];
	double eps_trace;

// Update strain
	for (int i = 0; i < 6; i++)
	{
		eps[i] = stran[i] + dstran[i];	
	}
	
	eps_trace = eps[0] + eps[1] + eps[2];

//Calculation of the stress
	for(int i = 0; i < 3; i++)
	{
		stress[i] = LAMBDA*eps_trace + 2.0*G*eps[i];
		stress[i+3] = G*eps[i+3];
	}

// Calc Isotropic elastic stiffness matrix through Eigen 
// (unnessary but informative)
	Eigen::Matrix<double, 6,6> C;
	CalcElaStiffMat(C, G, LAMBDA);

//	Isotropic elastic stiffness matrix ddsdde as a 36x1-Vector
//  But first it`s Initialization
	for (int i = 0; i < 6; i++)
	{
		for(int j = 0; j< 6; j++)
		{
			ddsdde[6*i+j] = 0.0; 
		}
	}
	for(int i = 0; i < 3; i++)
	{
		ddsdde[6*i+0] 			= C(0,i);
		ddsdde[6*i+1] 			= C(1,i);
		ddsdde[6*i+2] 			= C(2,i);
		ddsdde[6*(i+3)+(i+3)] 	= C(i+3, i+3);
	}
}