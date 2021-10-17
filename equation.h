
void solve_equation(double a,double b,double c,Complex &x1,Complex &x2){
	double delta=b*b-4.0*a*c;
	Complex son1=Complex(-b,0.0)+Sqrt(delta);
	Complex son2=Complex(-b,0.0)-Sqrt(delta);
	Complex mom=Complex(2.0*a,0.0);
	x1=son1/mom;
	x2=son2/mom;
}
