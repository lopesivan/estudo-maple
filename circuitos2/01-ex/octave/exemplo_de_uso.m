% The scale suffixes
% F  = 1E-15 % fento
% P  = 1E-12 % pico
% ksksks.m - ksksks
% Maintainer: Ivan Lopes

% N  = 1E-9  % nano
% U  = 1E-6  % micro
% MIL= 1E-3  % mile
% K  = 1E3   % kilo
% MEG= 1E6   % mega
% G  = 1E9   % giga
% T  = 1E12  % tera

% Exemplo de uso:
% V  = to_rd(10,30);  % <10, 30°>
% w  = 2*pi*f;
% XC = 1/(w*C);
% XL = w*L;
% ZL = to_rd(XL,90);
% ZC = to_rd(XC,-90);
% ZR = to_rd(R,0);
% Z  = ZR + ZC
% to_pd(Z)           % polar display
% I  = V/Z
% to_pd(I)           % polar display
% S  = V*conj(I)
% s  = abs(S)
% P  = real(S)
% Q  = imag(S)
% FP = P/s
