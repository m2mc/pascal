program fibonacci;

function fib(n : integer) : integer
begin
    if n = 0 then begin
        0
    end
    else begin
        if n = 1 then begin
            1
        end
        else begin
            fib(n - 2) + fib(n - 1)
        end
    end
end;

var x : integer;
begin
    x := 22;
    fib(x)
end