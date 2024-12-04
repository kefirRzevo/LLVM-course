; ModuleID = '/home/timur/timur/mygithub/LLVM-course/lang-to-ir/tests/app.cl'
source_filename = "/home/timur/timur/mygithub/LLVM-course/lang-to-ir/tests/app.cl"

@0 = private constant [512 x [512 x i32]] zeroinitializer

declare i32 @scan()

declare void @print(i32)

declare void @gui_init()

declare void @gui_exit()

declare void @gui_flush()

declare i32 @gui_quit_event()

declare void @gui_set_pixel(i32, i32, i32)

declare i32 @gui_rand()

define void @set_bound_cond(ptr %0) {
entry:
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  br label %cond

cond:                                             ; preds = %cont1, %entry
  %2 = load i32, ptr %1, align 4
  %3 = icmp ne i32 %2, 512
  %4 = icmp ne i1 %3, false
  br i1 %4, label %body, label %exit

body:                                             ; preds = %cond
  %5 = alloca i32, align 4
  %6 = call i32 @gui_rand()
  %7 = srem i32 %6, 2
  store i32 %7, ptr %5, align 4
  %8 = load i32, ptr %5, align 4
  %9 = icmp ne i32 %8, 0
  br i1 %9, label %then, label %else

exit:                                             ; preds = %cond
  br label %cont

cont:                                             ; preds = %exit
  ret void

then:                                             ; preds = %body
  %10 = load i32, ptr %1, align 4
  %11 = getelementptr i32, ptr %0, i32 %10
  store i32 1, ptr %11, align 4
  br label %cont1

else:                                             ; preds = %body
  %12 = load i32, ptr %1, align 4
  %13 = getelementptr i32, ptr %0, i32 %12
  store i32 0, ptr %13, align 4
  br label %cont1

cont1:                                            ; preds = %else, %then
  %14 = load i32, ptr %1, align 4
  %15 = add i32 %14, 1
  store i32 %15, ptr %1, align 4
  br label %cond
}

define void @apply_rule(ptr %0, ptr %1) {
entry:
  %2 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  br label %cond

cond:                                             ; preds = %cont7, %entry
  %3 = load i32, ptr %2, align 4
  %4 = icmp ne i32 %3, 512
  %5 = icmp ne i1 %4, false
  br i1 %5, label %body, label %exit

body:                                             ; preds = %cond
  %6 = alloca i32, align 4
  store i32 0, ptr %6, align 4
  %7 = load i32, ptr %2, align 4
  %8 = sub i32 %7, 1
  %9 = add i32 %8, 512
  %10 = srem i32 %9, 512
  %11 = getelementptr i32, ptr %0, i32 %10
  %12 = load i32, ptr %11, align 4
  %13 = icmp ne i32 %12, 0
  br i1 %13, label %then, label %cont1

exit:                                             ; preds = %cond
  br label %cont

cont:                                             ; preds = %exit
  ret void

then:                                             ; preds = %body
  %14 = load i32, ptr %6, align 4
  %15 = add i32 %14, 4
  store i32 %15, ptr %6, align 4
  br label %cont1

cont1:                                            ; preds = %then, %body
  %16 = load i32, ptr %2, align 4
  %17 = getelementptr i32, ptr %0, i32 %16
  %18 = load i32, ptr %17, align 4
  %19 = icmp ne i32 %18, 0
  br i1 %19, label %then2, label %cont3

then2:                                            ; preds = %cont1
  %20 = load i32, ptr %6, align 4
  %21 = add i32 %20, 2
  store i32 %21, ptr %6, align 4
  br label %cont3

cont3:                                            ; preds = %then2, %cont1
  %22 = load i32, ptr %2, align 4
  %23 = add i32 %22, 1
  %24 = srem i32 %23, 512
  %25 = getelementptr i32, ptr %0, i32 %24
  %26 = load i32, ptr %25, align 4
  %27 = icmp ne i32 %26, 0
  br i1 %27, label %then4, label %cont5

then4:                                            ; preds = %cont3
  %28 = load i32, ptr %6, align 4
  %29 = add i32 %28, 1
  store i32 %29, ptr %6, align 4
  br label %cont5

cont5:                                            ; preds = %then4, %cont3
  %30 = load i32, ptr %6, align 4
  %31 = shl i32 1, %30
  %32 = and i32 110, %31
  %33 = icmp ne i32 %32, 0
  br i1 %33, label %then6, label %else

then6:                                            ; preds = %cont5
  %34 = load i32, ptr %2, align 4
  %35 = getelementptr i32, ptr %1, i32 %34
  store i32 1, ptr %35, align 4
  br label %cont7

else:                                             ; preds = %cont5
  %36 = load i32, ptr %2, align 4
  %37 = getelementptr i32, ptr %1, i32 %36
  store i32 0, ptr %37, align 4
  br label %cont7

cont7:                                            ; preds = %else, %then6
  %38 = load i32, ptr %2, align 4
  %39 = add i32 %38, 1
  store i32 %39, ptr %2, align 4
  br label %cond
}

define void @main() {
entry:
  %0 = alloca i32, align 4
  store i32 0, ptr %0, align 4
  %1 = alloca [512 x [512 x i32]], align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @0, i64 1048576, i1 false)
  call void @gui_init()
  %2 = getelementptr [512 x [512 x i32]], ptr %1, i32 0
  %3 = getelementptr [512 x i32], ptr %2, i32 0
  %4 = getelementptr [512 x i32], ptr %3, i32 0
  call void @set_bound_cond(ptr %4)
  br label %cond

cond:                                             ; preds = %cont5, %entry
  br i1 true, label %body, label %exit

body:                                             ; preds = %cond
  %5 = alloca i32, align 4
  store i32 0, ptr %5, align 4
  %6 = call i32 @gui_quit_event()
  %7 = icmp ne i32 %6, 0
  br i1 %7, label %then, label %cont1

exit:                                             ; preds = %then, %cond
  br label %cont

cont:                                             ; preds = %exit
  call void @gui_exit()
  ret void

then:                                             ; preds = %body
  br label %exit

cont1:                                            ; preds = %body
  %8 = getelementptr [512 x [512 x i32]], ptr %1, i32 0
  %9 = load i32, ptr %0, align 4
  %10 = srem i32 %9, 512
  %11 = getelementptr [512 x i32], ptr %8, i32 %10
  %12 = getelementptr [512 x i32], ptr %11, i32 0
  %13 = getelementptr [512 x [512 x i32]], ptr %1, i32 0
  %14 = load i32, ptr %0, align 4
  %15 = add i32 %14, 1
  %16 = srem i32 %15, 512
  %17 = getelementptr [512 x i32], ptr %13, i32 %16
  %18 = getelementptr [512 x i32], ptr %17, i32 0
  call void @apply_rule(ptr %12, ptr %18)
  br label %cond2

cond2:                                            ; preds = %cont9, %cont1
  %19 = load i32, ptr %5, align 4
  %20 = icmp ne i32 %19, 512
  %21 = icmp ne i1 %20, false
  br i1 %21, label %body3, label %exit4

body3:                                            ; preds = %cond2
  %22 = alloca i32, align 4
  store i32 0, ptr %22, align 4
  br label %cond6

exit4:                                            ; preds = %cond2
  br label %cont5

cont5:                                            ; preds = %exit4
  call void @gui_flush()
  %23 = load i32, ptr %0, align 4
  %24 = add i32 %23, 1
  store i32 %24, ptr %0, align 4
  br label %cond

cond6:                                            ; preds = %cont11, %body3
  %25 = load i32, ptr %22, align 4
  %26 = icmp ne i32 %25, 512
  %27 = icmp ne i1 %26, false
  br i1 %27, label %body7, label %exit8

body7:                                            ; preds = %cond6
  %28 = getelementptr [512 x [512 x i32]], ptr %1, i32 0
  %29 = load i32, ptr %5, align 4
  %30 = getelementptr [512 x i32], ptr %28, i32 %29
  %31 = getelementptr [512 x i32], ptr %30, i32 0
  %32 = load i32, ptr %22, align 4
  %33 = getelementptr i32, ptr %31, i32 %32
  %34 = load i32, ptr %33, align 4
  %35 = icmp ne i32 %34, 0
  br i1 %35, label %then10, label %else

exit8:                                            ; preds = %cond6
  br label %cont9

cont9:                                            ; preds = %exit8
  %36 = load i32, ptr %5, align 4
  %37 = add i32 %36, 1
  store i32 %37, ptr %5, align 4
  br label %cond2

then10:                                           ; preds = %body7
  %38 = load i32, ptr %22, align 4
  %39 = load i32, ptr %5, align 4
  call void @gui_set_pixel(i32 %38, i32 %39, i32 255)
  br label %cont11

else:                                             ; preds = %body7
  %40 = load i32, ptr %22, align 4
  %41 = load i32, ptr %5, align 4
  call void @gui_set_pixel(i32 %40, i32 %41, i32 -1)
  br label %cont11

cont11:                                           ; preds = %else, %then10
  %42 = load i32, ptr %22, align 4
  %43 = add i32 %42, 1
  store i32 %43, ptr %22, align 4
  br label %cond6
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
