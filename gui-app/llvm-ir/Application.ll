; ModuleID = '/home/timur/timur/mygithub/LLVM-course/gui-app/src/Application.c'
source_filename = "/home/timur/timur/mygithub/LLVM-course/gui-app/src/Application.c"
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
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %13, label %68

13:                                               ; preds = %10, %51
  %14 = phi i32 [ %18, %51 ], [ 0, %10 ]
  %15 = and i32 %14, 511
  %16 = zext nneg i32 %15 to i64
  %17 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %16
  %18 = add nuw nsw i32 %14, 1
  %19 = and i32 %18, 511
  %20 = zext nneg i32 %19 to i64
  %21 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %20
  br label %22

22:                                               ; preds = %22, %13
  %23 = phi i64 [ 0, %13 ], [ %35, %22 ]
  %24 = add nuw nsw i64 %23, 511
  %25 = and i64 %24, 511
  %26 = getelementptr inbounds i8, ptr %17, i64 %25
  %27 = load i8, ptr %26, align 1, !tbaa !7, !range !11, !noundef !12
  %28 = icmp eq i8 %27, 0
  %29 = select i1 %28, i32 0, i32 4
  %30 = getelementptr inbounds i8, ptr %17, i64 %23
  %31 = load i8, ptr %30, align 1, !tbaa !7, !range !11, !noundef !12
  %32 = icmp eq i8 %31, 0
  %33 = or disjoint i32 %29, 2
  %34 = select i1 %32, i32 %29, i32 %33
  %35 = add nuw nsw i64 %23, 1
  %36 = and i64 %35, 511
  %37 = getelementptr inbounds i8, ptr %17, i64 %36
  %38 = load i8, ptr %37, align 1, !tbaa !7, !range !11, !noundef !12
  %39 = zext nneg i8 %38 to i32
  %40 = or disjoint i32 %34, %39
  %41 = shl nuw nsw i32 1, %40
  %42 = and i32 %41, 110
  %43 = icmp ne i32 %42, 0
  %44 = zext i1 %43 to i8
  %45 = getelementptr inbounds i8, ptr %21, i64 %23
  store i8 %44, ptr %45, align 1
  %46 = icmp eq i64 %35, 512
  br i1 %46, label %47, label %22, !llvm.loop !15

47:                                               ; preds = %22, %54
  %48 = phi i64 [ %55, %54 ], [ 0, %22 ]
  %49 = trunc i64 %48 to i32
  %50 = trunc i64 %48 to i32
  br label %57

51:                                               ; preds = %54
  tail call void @gui_flush() #5
  %52 = tail call i32 @gui_quit_event() #5
  %53 = icmp eq i32 %52, 0
  br i1 %53, label %13, label %68

54:                                               ; preds = %65
  %55 = add nuw nsw i64 %48, 1
  %56 = icmp eq i64 %55, 512
  br i1 %56, label %51, label %47, !llvm.loop !16

57:                                               ; preds = %47, %65
  %58 = phi i64 [ 0, %47 ], [ %66, %65 ]
  %59 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %48, i64 %58
  %60 = load i8, ptr %59, align 1, !tbaa !7, !range !11, !noundef !12
  %61 = icmp eq i8 %60, 0
  %62 = trunc i64 %58 to i32
  br i1 %61, label %64, label %63

63:                                               ; preds = %57
  tail call void @gui_set_pixel(i32 noundef %62, i32 noundef %49, i32 noundef 255) #5
  br label %65

64:                                               ; preds = %57
  tail call void @gui_set_pixel(i32 noundef %62, i32 noundef %50, i32 noundef -1) #5
  br label %65

65:                                               ; preds = %63, %64
  %66 = add nuw nsw i64 %58, 1
  %67 = icmp eq i64 %66, 512
  br i1 %67, label %54, label %57, !llvm.loop !17

68:                                               ; preds = %51, %10
  call void @llvm.lifetime.end.p0(i64 262144, ptr nonnull %1) #5
  ret void
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
!15 = distinct !{!15, !6, !14}
!16 = distinct !{!16, !6}
!17 = distinct !{!17, !6}
