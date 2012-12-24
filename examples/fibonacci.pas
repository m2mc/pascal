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
    while x < 23 do begin
        write(x);
        write(fib(x));
        write("");
        inc(x)
    end
end.