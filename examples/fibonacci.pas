program fibonacci;

{ very slow! }
function fib(n : integer) : integer
begin
    if n = 0 then begin
        fib := 0
    end
    else
        if n = 1 then begin
            fib := 1
        end
        else fib := fib(n - 2) + fib(n - 1)
end;

var x : integer;
begin
    x := 0;
    while x < 10 do begin
        write(fib(x));
        inc(x)
    end
end.