; ModuleID = '../src/Application.c'
source_filename = "../src/Application.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @set_bound_cond(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  store i32 0, ptr %3, align 4
  br label %5

5:                                                ; preds = %24, %1
  %6 = load i32, ptr %3, align 4
  %7 = icmp ne i32 %6, 512
  br i1 %7, label %8, label %27

8:                                                ; preds = %5
  %9 = call i32 @gui_rand()
  %10 = srem i32 %9, 2
  store i32 %10, ptr %4, align 4
  %11 = load i32, ptr %4, align 4
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %18

13:                                               ; preds = %8
  %14 = load ptr, ptr %2, align 8
  %15 = load i32, ptr %3, align 4
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds i8, ptr %14, i64 %16
  store i8 1, ptr %17, align 1
  br label %23

18:                                               ; preds = %8
  %19 = load ptr, ptr %2, align 8
  %20 = load i32, ptr %3, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds i8, ptr %19, i64 %21
  store i8 0, ptr %22, align 1
  br label %23

23:                                               ; preds = %18, %13
  br label %24

24:                                               ; preds = %23
  %25 = load i32, ptr %3, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr %3, align 4
  br label %5, !llvm.loop !6

27:                                               ; preds = %5
  ret void
}

declare i32 @gui_rand() #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @apply_rule(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  store i32 0, ptr %5, align 4
  br label %7

7:                                                ; preds = %61, %2
  %8 = load i32, ptr %5, align 4
  %9 = icmp ne i32 %8, 512
  br i1 %9, label %10, label %64

10:                                               ; preds = %7
  store i32 0, ptr %6, align 4
  %11 = load ptr, ptr %3, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sub nsw i32 %12, 1
  %14 = add nsw i32 %13, 512
  %15 = srem i32 %14, 512
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds i8, ptr %11, i64 %16
  %18 = load i8, ptr %17, align 1
  %19 = trunc i8 %18 to i1
  br i1 %19, label %20, label %23

20:                                               ; preds = %10
  %21 = load i32, ptr %6, align 4
  %22 = add nsw i32 %21, 4
  store i32 %22, ptr %6, align 4
  br label %23

23:                                               ; preds = %20, %10
  %24 = load ptr, ptr %3, align 8
  %25 = load i32, ptr %5, align 4
  %26 = sext i32 %25 to i64
  %27 = getelementptr inbounds i8, ptr %24, i64 %26
  %28 = load i8, ptr %27, align 1
  %29 = trunc i8 %28 to i1
  br i1 %29, label %30, label %33

30:                                               ; preds = %23
  %31 = load i32, ptr %6, align 4
  %32 = add nsw i32 %31, 2
  store i32 %32, ptr %6, align 4
  br label %33

33:                                               ; preds = %30, %23
  %34 = load ptr, ptr %3, align 8
  %35 = load i32, ptr %5, align 4
  %36 = add nsw i32 %35, 1
  %37 = srem i32 %36, 512
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds i8, ptr %34, i64 %38
  %40 = load i8, ptr %39, align 1
  %41 = trunc i8 %40 to i1
  br i1 %41, label %42, label %45

42:                                               ; preds = %33
  %43 = load i32, ptr %6, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, ptr %6, align 4
  br label %45

45:                                               ; preds = %42, %33
  %46 = load i32, ptr %6, align 4
  %47 = shl i32 1, %46
  %48 = and i32 110, %47
  %49 = icmp ne i32 %48, 0
  br i1 %49, label %50, label %55

50:                                               ; preds = %45
  %51 = load ptr, ptr %4, align 8
  %52 = load i32, ptr %5, align 4
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds i8, ptr %51, i64 %53
  store i8 1, ptr %54, align 1
  br label %60

55:                                               ; preds = %45
  %56 = load ptr, ptr %4, align 8
  %57 = load i32, ptr %5, align 4
  %58 = sext i32 %57 to i64
  %59 = getelementptr inbounds i8, ptr %56, i64 %58
  store i8 0, ptr %59, align 1
  br label %60

60:                                               ; preds = %55, %50
  br label %61

61:                                               ; preds = %60
  %62 = load i32, ptr %5, align 4
  %63 = add nsw i32 %62, 1
  store i32 %63, ptr %5, align 4
  br label %7, !llvm.loop !8

64:                                               ; preds = %7
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @app(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca [512 x [512 x i8]], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store ptr %1, ptr %4, align 8
  call void @llvm.memset.p0.i64(ptr align 16 %5, i8 0, i64 262144, i1 false)
  %9 = getelementptr inbounds [512 x [512 x i8]], ptr %5, i64 0, i64 0
  %10 = getelementptr inbounds [512 x i8], ptr %9, i64 0, i64 0
  call void @set_bound_cond(ptr noundef %10)
  store i32 0, ptr %6, align 4
  br label %11

11:                                               ; preds = %56, %2
  %12 = call zeroext i1 @gui_quit_event()
  br i1 %12, label %13, label %14

13:                                               ; preds = %11
  br label %59

14:                                               ; preds = %11
  %15 = load i32, ptr %6, align 4
  %16 = srem i32 %15, 512
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds [512 x [512 x i8]], ptr %5, i64 0, i64 %17
  %19 = getelementptr inbounds [512 x i8], ptr %18, i64 0, i64 0
  %20 = load i32, ptr %6, align 4
  %21 = add nsw i32 %20, 1
  %22 = srem i32 %21, 512
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds [512 x [512 x i8]], ptr %5, i64 0, i64 %23
  %25 = getelementptr inbounds [512 x i8], ptr %24, i64 0, i64 0
  call void @apply_rule(ptr noundef %19, ptr noundef %25)
  store i32 0, ptr %7, align 4
  br label %26

26:                                               ; preds = %53, %14
  %27 = load i32, ptr %7, align 4
  %28 = icmp ne i32 %27, 512
  br i1 %28, label %29, label %56

29:                                               ; preds = %26
  store i32 0, ptr %8, align 4
  br label %30

30:                                               ; preds = %49, %29
  %31 = load i32, ptr %8, align 4
  %32 = icmp ne i32 %31, 512
  br i1 %32, label %33, label %52

33:                                               ; preds = %30
  %34 = load i32, ptr %7, align 4
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds [512 x [512 x i8]], ptr %5, i64 0, i64 %35
  %37 = load i32, ptr %8, align 4
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds [512 x i8], ptr %36, i64 0, i64 %38
  %40 = load i8, ptr %39, align 1
  %41 = trunc i8 %40 to i1
  br i1 %41, label %42, label %45

42:                                               ; preds = %33
  %43 = load i32, ptr %8, align 4
  %44 = load i32, ptr %7, align 4
  call void @gui_set_pixel(i32 noundef %43, i32 noundef %44, i32 noundef 255)
  br label %48

45:                                               ; preds = %33
  %46 = load i32, ptr %8, align 4
  %47 = load i32, ptr %7, align 4
  call void @gui_set_pixel(i32 noundef %46, i32 noundef %47, i32 noundef -1)
  br label %48

48:                                               ; preds = %45, %42
  br label %49

49:                                               ; preds = %48
  %50 = load i32, ptr %8, align 4
  %51 = add nsw i32 %50, 1
  store i32 %51, ptr %8, align 4
  br label %30, !llvm.loop !9

52:                                               ; preds = %30
  br label %53

53:                                               ; preds = %52
  %54 = load i32, ptr %7, align 4
  %55 = add nsw i32 %54, 1
  store i32 %55, ptr %7, align 4
  br label %26, !llvm.loop !10

56:                                               ; preds = %26
  call void @gui_flush()
  %57 = load i32, ptr %6, align 4
  %58 = add nsw i32 %57, 1
  store i32 %58, ptr %6, align 4
  br label %11

59:                                               ; preds = %13
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

declare zeroext i1 @gui_quit_event() #1

declare void @gui_set_pixel(i32 noundef, i32 noundef, i32 noundef) #1

declare void @gui_flush() #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
