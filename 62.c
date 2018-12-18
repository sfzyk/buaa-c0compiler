int power(int a){
	if(a<=1){
		return (1);
	}
	return (a*power(a-1));
}

void main(){
	int b;
	scanf(b);
	printf(power(b));
} 
