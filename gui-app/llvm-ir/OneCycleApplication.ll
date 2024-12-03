; ModuleID = '/home/timur/timur/mygithub/LLVM-course/gui-app/src/OneCycleApplication.c'
source_filename = "/home/timur/timur/mygithub/LLVM-course/gui-app/src/OneCycleApplication.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @set_bound_cond(ptr nocapture noundef writeonly %0) local_unnamed_addr #0 {
  br label %3

2:                                                ; preds = %3
  ret void

3:                                                ; preds = %1, %3
  %4 = phi i64 [ 0, %1 ], [ %9, %3 ]
  %5 = tail call i32 @gui_rand() #5
  %6 = trunc i32 %5 to i8
  %7 = and i8 %6, 1
  %8 = getelementptr inbounds i8, ptr %0, i64 %4
  store i8 %7, ptr %8, align 1
  %9 = add nuw nsw i64 %4, 1
  %10 = icmp eq i64 %9, 512
  br i1 %10, label %2, label %3, !llvm.loop !5
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

declare i32 @gui_rand() local_unnamed_addr #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @apply_rule(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1) local_unnamed_addr #3 {
  br label %4

3:                                                ; preds = %4
  ret void

4:                                                ; preds = %2, %4
  %5 = phi i64 [ 0, %2 ], [ %17, %4 ]
  %6 = add nuw i64 %5, 511
  %7 = and i64 %6, 511
  %8 = getelementptr inbounds i8, ptr %0, i64 %7
  %9 = load i8, ptr %8, align 1, !tbaa !7, !range !11, !noundef !12
  %10 = icmp eq i8 %9, 0
  %11 = select i1 %10, i32 0, i32 4
  %12 = getelementptr inbounds i8, ptr %0, i64 %5
  %13 = load i8, ptr %12, align 1, !tbaa !7, !range !11, !noundef !12
  %14 = icmp eq i8 %13, 0
  %15 = or disjoint i32 %11, 2
  %16 = select i1 %14, i32 %11, i32 %15
  %17 = add nuw nsw i64 %5, 1
  %18 = and i64 %17, 511
  %19 = getelementptr inbounds i8, ptr %0, i64 %18
  %20 = load i8, ptr %19, align 1, !tbaa !7, !range !11, !noundef !12
  %21 = zext nneg i8 %20 to i32
  %22 = or disjoint i32 %16, %21
  %23 = shl nuw nsw i32 1, %22
  %24 = and i32 %23, 110
  %25 = icmp ne i32 %24, 0
  %26 = zext i1 %25 to i8
  %27 = getelementptr inbounds i8, ptr %1, i64 %5
  store i8 %26, ptr %27, align 1
  %28 = icmp eq i64 %17, 512
  br i1 %28, label %3, label %4, !llvm.loop !13
}

; Function Attrs: nounwind uwtable
define dso_local void @app() local_unnamed_addr #0 {
  %1 = alloca [512 x [512 x i8]], align 16
  call void @llvm.lifetime.start.p0(i64 262144, ptr nonnull %1) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(262144) %1, i8 0, i64 262144, i1 false)
  br label %2

2:                                                ; preds = %2, %0
  %3 = phi i64 [ 0, %0 ], [ %8, %2 ]
  %4 = tail call i32 @gui_rand() #5
  %5 = trunc i32 %4 to i8
  %6 = and i8 %5, 1
  %7 = getelementptr inbounds i8, ptr %1, i64 %3
  store i8 %6, ptr %7, align 1
  %8 = add nuw nsw i64 %3, 1
  %9 = icmp eq i64 %8, 512
  br i1 %9, label %10, label %2, !llvm.loop !5

10:                                               ; preds = %2
  %11 = tail call i32 @gui_quit_event() #5
  %12 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 1
  br label %13

13:                                               ; preds = %13, %10
  %14 = phi i64 [ 0, %10 ], [ %26, %13 ]
  %15 = add nuw nsw i64 %14, 511
  %16 = and i64 %15, 511
  %17 = getelementptr inbounds i8, ptr %1, i64 %16
  %18 = load i8, ptr %17, align 1, !tbaa !7, !range !11, !noundef !12
  %19 = icmp eq i8 %18, 0
  %20 = select i1 %19, i32 0, i32 4
  %21 = getelementptr inbounds i8, ptr %1, i64 %14
  %22 = load i8, ptr %21, align 1, !tbaa !7, !range !11, !noundef !12
  %23 = icmp eq i8 %22, 0
  %24 = or disjoint i32 %20, 2
  %25 = select i1 %23, i32 %20, i32 %24
  %26 = add nuw nsw i64 %14, 1
  %27 = and i64 %26, 511
  %28 = getelementptr inbounds i8, ptr %1, i64 %27
  %29 = load i8, ptr %28, align 1, !tbaa !7, !range !11, !noundef !12
  %30 = zext nneg i8 %29 to i32
  %31 = or disjoint i32 %25, %30
  %32 = shl nuw nsw i32 1, %31
  %33 = and i32 %32, 110
  %34 = icmp ne i32 %33, 0
  %35 = zext i1 %34 to i8
  %36 = getelementptr inbounds i8, ptr %12, i64 %14
  store i8 %35, ptr %36, align 1
  %37 = icmp eq i64 %26, 512
  br i1 %37, label %38, label %13, !llvm.loop !15

38:                                               ; preds = %13, %43
  %39 = phi i64 [ %44, %43 ], [ 0, %13 ]
  %40 = trunc i64 %39 to i32
  %41 = trunc i64 %39 to i32
  br label %46

42:                                               ; preds = %43
  tail call void @gui_flush() #5
  call void @llvm.lifetime.end.p0(i64 262144, ptr nonnull %1) #5
  ret void

43:                                               ; preds = %54
  %44 = add nuw nsw i64 %39, 1
  %45 = icmp eq i64 %44, 512
  br i1 %45, label %42, label %38, !llvm.loop !16

46:                                               ; preds = %38, %54
  %47 = phi i64 [ 0, %38 ], [ %55, %54 ]
  %48 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %39, i64 %47
  %49 = load i8, ptr %48, align 1, !tbaa !7, !range !11, !noundef !12
  %50 = icmp eq i8 %49, 0
  %51 = trunc i64 %47 to i32
  br i1 %50, label %53, label %52

52:                                               ; preds = %46
  tail call void @gui_set_pixel(i32 noundef %51, i32 noundef %40, i32 noundef 255) #5
  br label %54

53:                                               ; preds = %46
  tail call void @gui_set_pixel(i32 noundef %51, i32 noundef %41, i32 noundef -1) #5
  br label %54

54:                                               ; preds = %52, %53
  %55 = add nuw nsw i64 %47, 1
  %56 = icmp eq i64 %55, 512
  br i1 %56, label %43, label %46, !llvm.loop !17
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

declare i32 @gui_quit_event() local_unnamed_addr #2

declare void @gui_set_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #2

declare void @gui_flush() local_unnamed_addr #2

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!8, !8, i64 0}
!8 = !{!"bool", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
!11 = !{i8 0, i8 2}
!12 = !{}
!13 = distinct !{!13, !6, !14}
!14 = !{!"llvm.loop.isvectorized", i32 1}
!15 = distinct !{!15, !6}
!16 = distinct !{!16, !6}
!17 = distinct !{!17, !6}
