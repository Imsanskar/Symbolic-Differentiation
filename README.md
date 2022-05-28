# Symbolic Diffferentation

### Output
```
derivative(variable:"x") float func(float x){
    return sin(exp(cos(x)));
}

float _deriv_func (float x) {
	return ((cos ((exp ((cos (x)))))) * (((exp ((cos (x)))) * (((0 - ((sin (x)))) * (1))))));
}
```