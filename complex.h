#include<cmath>

class Complex{
	private:
		double real,imag;
	public:
		Complex(double x=0,double y=0){real=x;imag=y;}
		double x() const{return real;}
		double y() const{return imag;}
		Complex operator +(Complex const &obj){
			return Complex(real+obj.real,imag+obj.imag);
		}
		Complex operator -(Complex const &obj){
			return Complex(real-obj.real,imag-obj.imag);
		}
		Complex operator *(Complex const &obj){
			return Complex(real*obj.real-imag*obj.imag,real*obj.imag+imag*obj.real);
		}
		Complex operator /(Complex const &obj){
			return Complex( (real*obj.real+imag*obj.imag)/(obj.mag()*obj.mag()),(imag*obj.real-real*obj.imag)/(obj.mag()*obj.mag()) );
		}
		Complex pow(int x) const;
		double mag() const;
		double arg() const;
		Complex cjg() const;
		void out() const{
			printf("%.6lf%c%.6lfi",real,imag>=0?'+':'-',fabs(imag));
		}
};

Complex Complex::pow(int x) const{
	Complex res(1,0);
	Complex p(real,imag);
	for(int i=1;i<=abs(x);i++){
		res=res*p;
	}
	if(x<0) res=Complex(1,0)/res;
	return res;
}

double Complex::mag() const{
	return sqrt(real*real+imag*imag);
}

double Complex::arg() const{
	double res=acos(real/mag());
	if(imag>0) return res;
	else return -res;
}

Complex Complex::cjg() const{
	return Complex(real,-imag);
}

Complex dis(Complex a,Complex b){
	return Complex((a-b).mag(),0.0);
}

Complex Sqrt(double x){
	double y=sqrt(fabs(x));
	Complex res;
	if(x>0) res=Complex(y,0.0);
	else res=Complex(0.0,y);
	return res;
}
