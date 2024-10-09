; ModuleID = 'gui-app/src/OneCycleApplication.c'
source_filename = "gui-app/src/OneCycleApplication.c"
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
define dso_local void @app(i32 noundef %0, ptr nocapture noundef readnone %1) local_unnamed_addr #0 {
  %3 = alloca [512 x [512 x i8]], align 16
  call void @llvm.lifetime.start.p0(i64 262144, ptr nonnull %3) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(262144) %3, i8 0, i64 262144, i1 false)
  br label %4

4:                                                ; preds = %4, %2
  %5 = phi i64 [ 0, %2 ], [ %10, %4 ]
  %6 = tail call i32 @gui_rand() #5
  %7 = trunc i32 %6 to i8
  %8 = and i8 %7, 1
  %9 = getelementptr inbounds i8, ptr %3, i64 %5
  store i8 %8, ptr %9, align 1
  %10 = add nuw nsw i64 %5, 1
  %11 = icmp eq i64 %10, 512
  br i1 %11, label %12, label %4, !llvm.loop !5

12:                                               ; preds = %4
  %13 = tail call zeroext i1 @gui_quit_event() #5
  %14 = getelementptr inbounds [512 x [512 x i8]], ptr %3, i64 0, i64 1
  br label %15

15:                                               ; preds = %15, %12
  %16 = phi i64 [ 0, %12 ], [ %28, %15 ]
  %17 = add nuw nsw i64 %16, 511
  %18 = and i64 %17, 511
  %19 = getelementptr inbounds i8, ptr %3, i64 %18
  %20 = load i8, ptr %19, align 1, !tbaa !7, !range !11, !noundef !12
  %21 = icmp eq i8 %20, 0
  %22 = select i1 %21, i32 0, i32 4
  %23 = getelementptr inbounds i8, ptr %3, i64 %16
  %24 = load i8, ptr %23, align 1, !tbaa !7, !range !11, !noundef !12
  %25 = icmp eq i8 %24, 0
  %26 = or disjoint i32 %22, 2
  %27 = select i1 %25, i32 %22, i32 %26
  %28 = add nuw nsw i64 %16, 1
  %29 = and i64 %28, 511
  %30 = getelementptr inbounds i8, ptr %3, i64 %29
  %31 = load i8, ptr %30, align 1, !tbaa !7, !range !11, !noundef !12
  %32 = zext nneg i8 %31 to i32
  %33 = or disjoint i32 %27, %32
  %34 = shl nuw nsw i32 1, %33
  %35 = and i32 %34, 110
  %36 = icmp ne i32 %35, 0
  %37 = zext i1 %36 to i8
  %38 = getelementptr inbounds i8, ptr %14, i64 %16
  store i8 %37, ptr %38, align 1
  %39 = icmp eq i64 %28, 512
  br i1 %39, label %40, label %15, !llvm.loop !15

40:                                               ; preds = %15, %45
  %41 = phi i64 [ %46, %45 ], [ 0, %15 ]
  %42 = trunc i64 %41 to i32
  %43 = trunc i64 %41 to i32
  br label %48

44:                                               ; preds = %45
  tail call void @gui_flush() #5
  call void @llvm.lifetime.end.p0(i64 262144, ptr nonnull %3) #5
  ret void

45:                                               ; preds = %56
  %46 = add nuw nsw i64 %41, 1
  %47 = icmp eq i64 %46, 512
  br i1 %47, label %44, label %40, !llvm.loop !16

48:                                               ; preds = %40, %56
  %49 = phi i64 [ 0, %40 ], [ %57, %56 ]
  %50 = getelementptr inbounds [512 x [512 x i8]], ptr %3, i64 0, i64 %41, i64 %49
  %51 = load i8, ptr %50, align 1, !tbaa !7, !range !11, !noundef !12
  %52 = icmp eq i8 %51, 0
  %53 = trunc i64 %49 to i32
  br i1 %52, label %55, label %54

54:                                               ; preds = %48
  tail call void @gui_set_pixel(i32 noundef %53, i32 noundef %42, i32 noundef 255) #5
  br label %56

55:                                               ; preds = %48
  tail call void @gui_set_pixel(i32 noundef %53, i32 noundef %43, i32 noundef -1) #5
  br label %56

56:                                               ; preds = %54, %55
  %57 = add nuw nsw i64 %49, 1
  %58 = icmp eq i64 %57, 512
  br i1 %58, label %45, label %48, !llvm.loop !17
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
!15 = distinct !{!15, !6}
!16 = distinct !{!16, !6}
!17 = distinct !{!17, !6}
