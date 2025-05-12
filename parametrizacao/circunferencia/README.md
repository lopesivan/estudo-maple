Vamos **parametrizar a circunferência** de raio $r$, com
centro na origem $(0,0)$, para os dois sentidos de rotação:

---

## 🧭 1. Sentido **anti-horário** (trigonométrico)

Essa é a forma **padrão** de parametrizar:

$$
\boxed{
\vec{\gamma}(t) = (r \cos t,\ r \sin t), \quad t \in [0, 2\pi]
}
$$

* Começa no ponto $(r, 0)$,
* Passa por $(0, r)$, depois $(-r, 0)$, etc.,
* Gira no sentido **anti-horário** (positivo, padrão trigonométrico).

---

## 🕓 2. Sentido **horário** (sentido do relógio)

Basta **inverter o sinal do seno** (ou inverter o tempo):

$$
\boxed{
\vec{\gamma}(t) = (r \cos t,\ -r \sin t), \quad t \in [0, 2\pi]
}
$$

* Começa também em $(r, 0)$,
* Mas gira no **sentido horário**: descendo para $(0, -r)$, etc.

---

## 🧠 Alternativa equivalente: inverter o tempo

Outra forma de inverter o sentido é usar:

$$
\vec{\gamma}(t) = (r \cos(-t),\ r \sin(-t)) = (r \cos t,\ -r \sin t)
$$

(visto que $\cos(-t) = \cos t$ e $\sin(-t) = -\sin t$)

---

## 🎯 Resumo

| Sentido                       | Parametrização        |
| :---------------------------- | :-------------------- |
| Trigonométrico (anti-horário) | $(r\cos t, r\sin t)$  |
| Horário (sentido do relógio)  | $(r\cos t, -r\sin t)$ |

---

