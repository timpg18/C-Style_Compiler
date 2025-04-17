int main(){
    int x= 0;
    x++;

    {
        int x;
        x++;
        int y;
    }
    {
        int x;
        int y;
        x++;
        {
            int x;
            x++;
            {  
                y++;
                x++;
                int x;
                x++;
            }
        }
    }
}