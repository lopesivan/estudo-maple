interface(imaginaryunit= j);
with(plots):

# Definindo os fasores como números complexos

R:=3;
XL:=2;
XC:=6;
X:=XL-XC;
fase:=10; # graus

Vs_ := 10*exp(j*fase*Pi/180);
argument(Vs_);
Z_  := R-X*j;
evalf(argument(Z_)*180/Pi);
I_  := (Vs_/Z_);
evalf(argument(I_)*180/Pi);


VR := R * I_;
evalc(VR);
VL := XL * I_;                # VL = jωL * I
evalc(VL);

# I_ := 2*exp(-j*53.13*Pi/180):        # 2 ∠ -53.13°

# Verificações
# evalc(VR + VL);                     # deve ser igual a Vs

