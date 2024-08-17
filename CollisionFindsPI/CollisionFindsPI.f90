program CollisionFindsPI
    implicit none

    integer::m1 = 1
    integer::m2 = 100
    integer::n
    real::v1,x1,v1_
    real::v2,x2,v2_
    real::dt=0.0001
    do
        n=0
        v1=0
        v2=-1
        x1=2
        x2=3
        do
            if (v2>v1 .and. v1>0) then
                exit
            end if
            x1=x1+v1*dt
            x2=x2+v2*dt
            if(x1>x2) then
                v1_ = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2)
                v2_ = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2)
                v1 = v1_
                v2 = v2_
                n =n + 1
            end if
            if(x1<1) then
                v1=-v1
                n=n+1
            end if
        end do
        print *,m2," ",n
        m2=m2*100
    end do

end program CollisionFindsPI
