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
  %11 = tail call zeroext i1 @gui_quit_event() #5
  br i1 %11, label %66, label %12

12:                                               ; preds = %10, %50
  %13 = phi i32 [ %17, %50 ], [ 0, %10 ]
  %14 = and i32 %13, 511
  %15 = zext nneg i32 %14 to i64
  %16 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %15
  %17 = add nuw nsw i32 %13, 1
  %18 = and i32 %17, 511
  %19 = zext nneg i32 %18 to i64
  %20 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %19
  br label %21

21:                                               ; preds = %21, %12
  %22 = phi i64 [ 0, %12 ], [ %34, %21 ]
  %23 = add nuw nsw i64 %22, 511
  %24 = and i64 %23, 511
  %25 = getelementptr inbounds i8, ptr %16, i64 %24
  %26 = load i8, ptr %25, align 1, !tbaa !7, !range !11, !noundef !12
  %27 = icmp eq i8 %26, 0
  %28 = select i1 %27, i32 0, i32 4
  %29 = getelementptr inbounds i8, ptr %16, i64 %22
  %30 = load i8, ptr %29, align 1, !tbaa !7, !range !11, !noundef !12
  %31 = icmp eq i8 %30, 0
  %32 = or disjoint i32 %28, 2
  %33 = select i1 %31, i32 %28, i32 %32
  %34 = add nuw nsw i64 %22, 1
  %35 = and i64 %34, 511
  %36 = getelementptr inbounds i8, ptr %16, i64 %35
  %37 = load i8, ptr %36, align 1, !tbaa !7, !range !11, !noundef !12
  %38 = zext nneg i8 %37 to i32
  %39 = or disjoint i32 %33, %38
  %40 = shl nuw nsw i32 1, %39
  %41 = and i32 %40, 110
  %42 = icmp ne i32 %41, 0
  %43 = zext i1 %42 to i8
  %44 = getelementptr inbounds i8, ptr %20, i64 %22
  store i8 %43, ptr %44, align 1
  %45 = icmp eq i64 %34, 512
  br i1 %45, label %46, label %21, !llvm.loop !15

46:                                               ; preds = %21, %52
  %47 = phi i64 [ %53, %52 ], [ 0, %21 ]
  %48 = trunc i64 %47 to i32
  %49 = trunc i64 %47 to i32
  br label %55

50:                                               ; preds = %52
  tail call void @gui_flush() #5
  %51 = tail call zeroext i1 @gui_quit_event() #5
  br i1 %51, label %66, label %12

52:                                               ; preds = %63
  %53 = add nuw nsw i64 %47, 1
  %54 = icmp eq i64 %53, 512
  br i1 %54, label %50, label %46, !llvm.loop !16

55:                                               ; preds = %46, %63
  %56 = phi i64 [ 0, %46 ], [ %64, %63 ]
  %57 = getelementptr inbounds [512 x [512 x i8]], ptr %1, i64 0, i64 %47, i64 %56
  %58 = load i8, ptr %57, align 1, !tbaa !7, !range !11, !noundef !12
  %59 = icmp eq i8 %58, 0
  %60 = trunc i64 %56 to i32
  br i1 %59, label %62, label %61

61:                                               ; preds = %55
  tail call void @gui_set_pixel(i32 noundef %60, i32 noundef %48, i32 noundef 255) #5
  br label %63

62:                                               ; preds = %55
  tail call void @gui_set_pixel(i32 noundef %60, i32 noundef %49, i32 noundef -1) #5
  br label %63

63:                                               ; preds = %61, %62
  %64 = add nuw nsw i64 %56, 1
  %65 = icmp eq i64 %64, 512
  br i1 %65, label %52, label %55, !llvm.loop !17

66:                                               ; preds = %50, %10
  call void @llvm.lifetime.end.p0(i64 262144, ptr nonnull %1) #5
  ret void
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

declare zeroext i1 @gui_quit_event() local_unnamed_addr #2

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
