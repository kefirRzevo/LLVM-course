; ModuleID = 'gui-app/llvm-ir/OneCycleApplication.ll'
source_filename = "gui-app/src/OneCycleApplication.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"ret\00", align 1
@2 = private unnamed_addr constant [9 x i8] c"gui_rand\00", align 1
@3 = private unnamed_addr constant [15 x i8] c"set_bound_cond\00", align 1
@4 = private unnamed_addr constant [6 x i8] c"trunc\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@6 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@7 = private unnamed_addr constant [6 x i8] c"store\00", align 1
@8 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@9 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@10 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@11 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@12 = private unnamed_addr constant [4 x i8] c"ret\00", align 1
@13 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@14 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@15 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@16 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@17 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@18 = private unnamed_addr constant [7 x i8] c"select\00", align 1
@19 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@20 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@21 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@22 = private unnamed_addr constant [3 x i8] c"or\00", align 1
@23 = private unnamed_addr constant [7 x i8] c"select\00", align 1
@24 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@25 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@26 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@27 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@28 = private unnamed_addr constant [5 x i8] c"zext\00", align 1
@29 = private unnamed_addr constant [3 x i8] c"or\00", align 1
@30 = private unnamed_addr constant [4 x i8] c"shl\00", align 1
@31 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@32 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@33 = private unnamed_addr constant [5 x i8] c"zext\00", align 1
@34 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@35 = private unnamed_addr constant [6 x i8] c"store\00", align 1
@36 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@37 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@38 = private unnamed_addr constant [7 x i8] c"alloca\00", align 1
@39 = private unnamed_addr constant [23 x i8] c"llvm.lifetime.start.p0\00", align 1
@40 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@41 = private unnamed_addr constant [19 x i8] c"llvm.memset.p0.i64\00", align 1
@42 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@43 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@44 = private unnamed_addr constant [9 x i8] c"gui_rand\00", align 1
@45 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@46 = private unnamed_addr constant [6 x i8] c"trunc\00", align 1
@47 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@48 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@49 = private unnamed_addr constant [6 x i8] c"store\00", align 1
@50 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@51 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@52 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@53 = private unnamed_addr constant [15 x i8] c"gui_quit_event\00", align 1
@54 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@55 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@56 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@57 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@58 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@59 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@60 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@61 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@62 = private unnamed_addr constant [7 x i8] c"select\00", align 1
@63 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@64 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@65 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@66 = private unnamed_addr constant [3 x i8] c"or\00", align 1
@67 = private unnamed_addr constant [7 x i8] c"select\00", align 1
@68 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@69 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@70 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@71 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@72 = private unnamed_addr constant [5 x i8] c"zext\00", align 1
@73 = private unnamed_addr constant [3 x i8] c"or\00", align 1
@74 = private unnamed_addr constant [4 x i8] c"shl\00", align 1
@75 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@76 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@77 = private unnamed_addr constant [5 x i8] c"zext\00", align 1
@78 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@79 = private unnamed_addr constant [6 x i8] c"store\00", align 1
@80 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@81 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@82 = private unnamed_addr constant [6 x i8] c"trunc\00", align 1
@83 = private unnamed_addr constant [6 x i8] c"trunc\00", align 1
@84 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@85 = private unnamed_addr constant [10 x i8] c"gui_flush\00", align 1
@86 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@87 = private unnamed_addr constant [21 x i8] c"llvm.lifetime.end.p0\00", align 1
@88 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@89 = private unnamed_addr constant [4 x i8] c"ret\00", align 1
@90 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@91 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@92 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@93 = private unnamed_addr constant [14 x i8] c"getelementptr\00", align 1
@94 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@95 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@96 = private unnamed_addr constant [6 x i8] c"trunc\00", align 1
@97 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@98 = private unnamed_addr constant [14 x i8] c"gui_set_pixel\00", align 1
@99 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@100 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@101 = private unnamed_addr constant [14 x i8] c"gui_set_pixel\00", align 1
@102 = private unnamed_addr constant [4 x i8] c"app\00", align 1
@103 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@104 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@105 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@106 = private unnamed_addr constant [3 x i8] c"br\00", align 1

; Function Attrs: nounwind uwtable
define dso_local void @set_bound_cond(ptr nocapture noundef writeonly %0) local_unnamed_addr #0 {
  call void @optLogger(ptr @0)
  br label %3

2:                                                ; preds = %3
  call void @optLogger(ptr @1)
  ret void

3:                                                ; preds = %3, %1
  %4 = phi i64 [ 0, %1 ], [ %9, %3 ]
  call void @callOptLogger(ptr @3, ptr @2)
  %5 = tail call i32 @gui_rand() #5
  call void @optLogger(ptr @4)
  %6 = trunc i32 %5 to i8
  call void @optLogger(ptr @5)
  %7 = and i8 %6, 1
  call void @optLogger(ptr @6)
  %8 = getelementptr inbounds i8, ptr %0, i64 %4
  call void @optLogger(ptr @7)
  store i8 %7, ptr %8, align 1
  call void @optLogger(ptr @8)
  %9 = add nuw nsw i64 %4, 1
  call void @optLogger(ptr @9)
  %10 = icmp eq i64 %9, 512
  call void @optLogger(ptr @10)
  br i1 %10, label %2, label %3, !llvm.loop !5
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

declare i32 @gui_rand() local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @apply_rule(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1) local_unnamed_addr #3 {
  call void @optLogger(ptr @11)
  br label %4

3:                                                ; preds = %4
  call void @optLogger(ptr @12)
  ret void

4:                                                ; preds = %4, %2
  %5 = phi i64 [ 0, %2 ], [ %17, %4 ]
  call void @optLogger(ptr @13)
  %6 = add nuw i64 %5, 511
  call void @optLogger(ptr @14)
  %7 = and i64 %6, 511
  call void @optLogger(ptr @15)
  %8 = getelementptr inbounds i8, ptr %0, i64 %7
  call void @optLogger(ptr @16)
  %9 = load i8, ptr %8, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @17)
  %10 = icmp eq i8 %9, 0
  call void @optLogger(ptr @18)
  %11 = select i1 %10, i32 0, i32 4
  call void @optLogger(ptr @19)
  %12 = getelementptr inbounds i8, ptr %0, i64 %5
  call void @optLogger(ptr @20)
  %13 = load i8, ptr %12, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @21)
  %14 = icmp eq i8 %13, 0
  call void @optLogger(ptr @22)
  %15 = or disjoint i32 %11, 2
  call void @optLogger(ptr @23)
  %16 = select i1 %14, i32 %11, i32 %15
  call void @optLogger(ptr @24)
  %17 = add nuw nsw i64 %5, 1
  call void @optLogger(ptr @25)
  %18 = and i64 %17, 511
  call void @optLogger(ptr @26)
  %19 = getelementptr inbounds i8, ptr %0, i64 %18
  call void @optLogger(ptr @27)
  %20 = load i8, ptr %19, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @28)
  %21 = zext nneg i8 %20 to i32
  call void @optLogger(ptr @29)
  %22 = or disjoint i32 %16, %21
  call void @optLogger(ptr @30)
  %23 = shl nuw nsw i32 1, %22
  call void @optLogger(ptr @31)
  %24 = and i32 %23, 110
  call void @optLogger(ptr @32)
  %25 = icmp ne i32 %24, 0
  call void @optLogger(ptr @33)
  %26 = zext i1 %25 to i8
  call void @optLogger(ptr @34)
  %27 = getelementptr inbounds i8, ptr %1, i64 %5
  call void @optLogger(ptr @35)
  store i8 %26, ptr %27, align 1
  call void @optLogger(ptr @36)
  %28 = icmp eq i64 %17, 512
  call void @optLogger(ptr @37)
  br i1 %28, label %3, label %4, !llvm.loop !13
}

; Function Attrs: nounwind uwtable
define dso_local void @app(i32 noundef %0, ptr nocapture noundef readnone %1) local_unnamed_addr #0 {
  call void @optLogger(ptr @38)
  %3 = alloca [512 x [512 x i8]], align 16
  call void @callOptLogger(ptr @40, ptr @39)
  call void @llvm.lifetime.start.p0(i64 262144, ptr nonnull %3) #5
  call void @callOptLogger(ptr @42, ptr @41)
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(262144) %3, i8 0, i64 262144, i1 false)
  call void @optLogger(ptr @43)
  br label %4

4:                                                ; preds = %4, %2
  %5 = phi i64 [ 0, %2 ], [ %10, %4 ]
  call void @callOptLogger(ptr @45, ptr @44)
  %6 = tail call i32 @gui_rand() #5
  call void @optLogger(ptr @46)
  %7 = trunc i32 %6 to i8
  call void @optLogger(ptr @47)
  %8 = and i8 %7, 1
  call void @optLogger(ptr @48)
  %9 = getelementptr inbounds i8, ptr %3, i64 %5
  call void @optLogger(ptr @49)
  store i8 %8, ptr %9, align 1
  call void @optLogger(ptr @50)
  %10 = add nuw nsw i64 %5, 1
  call void @optLogger(ptr @51)
  %11 = icmp eq i64 %10, 512
  call void @optLogger(ptr @52)
  br i1 %11, label %12, label %4, !llvm.loop !5

12:                                               ; preds = %4
  call void @callOptLogger(ptr @54, ptr @53)
  %13 = tail call zeroext i1 @gui_quit_event() #5
  call void @optLogger(ptr @55)
  %14 = getelementptr inbounds [512 x [512 x i8]], ptr %3, i64 0, i64 1
  call void @optLogger(ptr @56)
  br label %15

15:                                               ; preds = %15, %12
  %16 = phi i64 [ 0, %12 ], [ %28, %15 ]
  call void @optLogger(ptr @57)
  %17 = add nuw nsw i64 %16, 511
  call void @optLogger(ptr @58)
  %18 = and i64 %17, 511
  call void @optLogger(ptr @59)
  %19 = getelementptr inbounds i8, ptr %3, i64 %18
  call void @optLogger(ptr @60)
  %20 = load i8, ptr %19, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @61)
  %21 = icmp eq i8 %20, 0
  call void @optLogger(ptr @62)
  %22 = select i1 %21, i32 0, i32 4
  call void @optLogger(ptr @63)
  %23 = getelementptr inbounds i8, ptr %3, i64 %16
  call void @optLogger(ptr @64)
  %24 = load i8, ptr %23, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @65)
  %25 = icmp eq i8 %24, 0
  call void @optLogger(ptr @66)
  %26 = or disjoint i32 %22, 2
  call void @optLogger(ptr @67)
  %27 = select i1 %25, i32 %22, i32 %26
  call void @optLogger(ptr @68)
  %28 = add nuw nsw i64 %16, 1
  call void @optLogger(ptr @69)
  %29 = and i64 %28, 511
  call void @optLogger(ptr @70)
  %30 = getelementptr inbounds i8, ptr %3, i64 %29
  call void @optLogger(ptr @71)
  %31 = load i8, ptr %30, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @72)
  %32 = zext nneg i8 %31 to i32
  call void @optLogger(ptr @73)
  %33 = or disjoint i32 %27, %32
  call void @optLogger(ptr @74)
  %34 = shl nuw nsw i32 1, %33
  call void @optLogger(ptr @75)
  %35 = and i32 %34, 110
  call void @optLogger(ptr @76)
  %36 = icmp ne i32 %35, 0
  call void @optLogger(ptr @77)
  %37 = zext i1 %36 to i8
  call void @optLogger(ptr @78)
  %38 = getelementptr inbounds i8, ptr %14, i64 %16
  call void @optLogger(ptr @79)
  store i8 %37, ptr %38, align 1
  call void @optLogger(ptr @80)
  %39 = icmp eq i64 %28, 512
  call void @optLogger(ptr @81)
  br i1 %39, label %40, label %15, !llvm.loop !15

40:                                               ; preds = %45, %15
  %41 = phi i64 [ %46, %45 ], [ 0, %15 ]
  call void @optLogger(ptr @82)
  %42 = trunc i64 %41 to i32
  call void @optLogger(ptr @83)
  %43 = trunc i64 %41 to i32
  call void @optLogger(ptr @84)
  br label %48

44:                                               ; preds = %45
  call void @callOptLogger(ptr @86, ptr @85)
  tail call void @gui_flush() #5
  call void @callOptLogger(ptr @88, ptr @87)
  call void @llvm.lifetime.end.p0(i64 262144, ptr nonnull %3) #5
  call void @optLogger(ptr @89)
  ret void

45:                                               ; preds = %56
  call void @optLogger(ptr @90)
  %46 = add nuw nsw i64 %41, 1
  call void @optLogger(ptr @91)
  %47 = icmp eq i64 %46, 512
  call void @optLogger(ptr @92)
  br i1 %47, label %44, label %40, !llvm.loop !16

48:                                               ; preds = %56, %40
  %49 = phi i64 [ 0, %40 ], [ %57, %56 ]
  call void @optLogger(ptr @93)
  %50 = getelementptr inbounds [512 x [512 x i8]], ptr %3, i64 0, i64 %41, i64 %49
  call void @optLogger(ptr @94)
  %51 = load i8, ptr %50, align 1, !tbaa !7, !range !11, !noundef !12
  call void @optLogger(ptr @95)
  %52 = icmp eq i8 %51, 0
  call void @optLogger(ptr @96)
  %53 = trunc i64 %49 to i32
  call void @optLogger(ptr @97)
  br i1 %52, label %55, label %54

54:                                               ; preds = %48
  call void @callOptLogger(ptr @99, ptr @98)
  tail call void @gui_set_pixel(i32 noundef %53, i32 noundef %42, i32 noundef 255) #5
  call void @optLogger(ptr @100)
  br label %56

55:                                               ; preds = %48
  call void @callOptLogger(ptr @102, ptr @101)
  tail call void @gui_set_pixel(i32 noundef %53, i32 noundef %43, i32 noundef -1) #5
  call void @optLogger(ptr @103)
  br label %56

56:                                               ; preds = %55, %54
  call void @optLogger(ptr @104)
  %57 = add nuw nsw i64 %49, 1
  call void @optLogger(ptr @105)
  %58 = icmp eq i64 %57, 512
  call void @optLogger(ptr @106)
  br i1 %58, label %45, label %48, !llvm.loop !17
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

declare zeroext i1 @gui_quit_event() local_unnamed_addr #2

declare void @gui_set_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #2

declare void @gui_flush() local_unnamed_addr #2

declare void @callOptLogger(ptr, ptr)

declare void @optLogger(ptr)

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: write) }
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
