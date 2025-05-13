restart:

# Agora carregue os pacotes
with(plots):
with(plottools):

# Raio da circunferência
r := 1:

# Parametrização horária
x := t -> r*cos(t):
y := t -> -r*sin(t):

# Intervalo de t
tmin := 0:
tmax := evalf((5*Pi)/6):
N    := 100000:
step := evalf((tmax - tmin)/N):

# Curva do arco
circ := plot([x(t), y(t), t = tmin .. tmax], color = blue, thickness = 2):

# Preenchimento da fatia
pontos := [seq([x(t), y(t)], t = tmin .. tmax, step)]:
preenchimento := polygon([[0, 0], op(pontos)], color = cyan, transparency = 0.3):

# COMANDO CRUCIAL: não usar ; nem : aqui! Essa linha renderiza e salva o gráfico
p := display([preenchimento, circ], scaling = constrained, axes = normal);
plotsetup(ps, plotoutput="04.ps", plotoptions="color,portrait,height=500,width=500");
print(p);
plotsetup(default):

