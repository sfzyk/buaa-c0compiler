float power(float a){
	if(a<=1){
		return (1.0);
	}
	return (a*power(a-1));
}

void main(){
	float b;
	scanf(b);
	printf(power(b));
} 
