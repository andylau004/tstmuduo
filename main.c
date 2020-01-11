

#include  <stdio.h>
//#include  <iostream>

int main ( int argc , void ** argv ) {
	        double R_Zero, R_PosInf, R_NegInf, R_Nan;
        /* Double constants initialization */
        R_Zero = 0.0;
        R_PosInf = 1.0/R_Zero;
        R_NegInf = -1.0/R_Zero;
        R_Nan = R_Zero/R_Zero;
//        std::cout << " not crash ............." << std::endl;
//        std::cout << "R_Zero = " << R_Zero << std::endl;
//        std::cout << "R_PosInf = " << R_PosInf << std::endl;
//        std::cout << "R_NegInf = " << R_NegInf << std::endl;
//        std::cout << "R_Nan = " << R_Nan << std::endl;

        printf( "R_PosInf=%lf\n", R_PosInf );

        return 1;
}

